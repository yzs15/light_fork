#include "ipdos/sharemalloc_dlmalloc.h"
#include "ipdos/ipdos.h"
#include "ipdos_util/info_util.h"
#include <assert.h>
#include <signal.h>
typedef void (*free_func_t)(void *);
typedef void *(*calloc_func_t)(size_t, size_t);
typedef void *(*realloc_func_t)(void *, size_t);
typedef void *(*malloc_func_t)(size_t);
typedef void *(*memalign_func_t)(size_t, size_t);
typedef int (*posix_memalign_func_t)(void **memptr, size_t alignment,
                                     size_t size);

static void *(*original_dlopen)(const char *__file, int __mode) = NULL;
mspace shared_mspace = NULL;
void *(*original_mmap)(void *addr, size_t length, int prot, int flags, int fd,
                       off_t offset);
int (*original_munmap)(void *addr, size_t length);
size_t (*original_malloc_usable_size)(void *ptr);
malloc_func_t real_malloc = NULL;
static pid_t (*real_fork)(void) = NULL;
static free_func_t real_free = NULL;
// alloca_func_t real_alloca = NULL;
typedef struct share_mmap_list {
    void *ptr;
    size_t length;
    int prot;
    int file_fd;
    int flags;
    off_t offset;
    struct share_mmap_list *next;
} share_mmap_list_t;

share_mmap_list_t
    *share_mmap_list_head; // 记录共享内存中的mmap列表，head自己不存储数据

typedef struct dl_open_list {
    void *file_name;
    int flag;
    struct dl_open_list *next;
} dl_open_list_t;

dl_open_list_t *dl_open_list_head;

static int provider_fd = -1;
void *shared_memory_ptr;
size_t shared_memory_alloc_size =
    0; // provider 使用的 当前已经分配的虚拟内存的大小

bool share_malloc_initialized = false;
bool use_share_malloc = true;
pthread_mutex_t lock;
bool load_from_shared_memory = false;

struct service_map_t *share_malloc_user_service_map = NULL;

// 设置是否从共享内存加载
bool set_load_from_shared_memory_env(bool env) {
    load_from_shared_memory = env;
    return true;
}
// 判断是否从共享内存加载
bool is_load_from_shared_memory() { return load_from_shared_memory; }

bool is_shared_malloc_used() {
    // LG("is_shared_malloc_used");
    return share_malloc_initialized & use_share_malloc;
}

bool is_provider = false;        // 是否是provider进程的标志
bool is_provider_forked = false; // 是否是provider fork出来的子进程

// 在后续.so加载前执行
void share_malloc_load_builtin_func() {
    original_mmap = dlsym(RTLD_NEXT, "mmap");
    original_munmap = dlsym(RTLD_NEXT, "munmap");
    original_malloc_usable_size = dlsym(RTLD_NEXT, "malloc_usable_size");
    real_malloc = (malloc_func_t)dlsym(RTLD_NEXT, "malloc");
    real_fork = (pid_t(*)(void))dlsym(RTLD_NEXT, "fork");
    real_free = (free_func_t)dlsym(RTLD_NEXT, "free");
    original_dlopen = dlsym(RTLD_NEXT, "dlopen");
    if (pthread_mutex_init(&lock, NULL) != 0) {
        LG("Mutex initialization failed\n");
        return;
    }
}

void share_malloc_provider_init(unsigned long start_addr, char *file_path) {
    is_provider = true;
    provider_fd =
        open(file_path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
    if (provider_fd == -1) {
        perror("shm_open");
        return;
    }
    if (ftruncate(provider_fd, SHARE_MALLOC_OFFEST) == -1) {
        perror("ftruncate");
        return;
    }

    struct stat file_stat;
    if (fstat(provider_fd, &file_stat) == -1) {
        perror("fstat");
        close(provider_fd);
        return;
    }
    void *baseaddr = (void *)(intptr_t)start_addr;

    shared_memory_ptr = original_mmap(baseaddr, SHARE_MALLOC_OFFEST,
                                      PROT_READ | PROT_WRITE | PROT_EXEC,
                                      MAP_SHARED | MAP_FIXED, provider_fd, 0);
    if (shared_memory_ptr == MAP_FAILED) {
        LG("mmap %s\n", strerror(errno));
        return;
    }
    return;
}

void shared_malloc_set_user_service_map(
    struct service_map_t *user_service_map) {
    share_malloc_user_service_map = user_service_map;
}

#define RTLD_SHARED 0x10000
#define RTLD_SHARED_REINIT 0x20000

char *MemFileName = NULL;

bool ipdos_set_mem_file_name(char *file_name) {
    MemFileName = file_name;
    return true;
}

int ipdos_remove_parent_mem_file() {}

// provider fork之后需要进行特殊处理
void sharemalloc_handle_fork(void) {
    if (!is_provider || !is_shared_malloc_used()) {
        // 说明不会使用从父进程继承的共享内存的share属性
        printf(
            "sharemalloc_handle_fork not provider or not use share malloc\n");
        return;
    }

    // printf("%p\n", &is_provider_forked);
    void *baseaddr = shared_memory_ptr;
    original_munmap(baseaddr, shared_memory_alloc_size);
    void *shared_mem = original_mmap(baseaddr, shared_memory_alloc_size,
                                     PROT_READ | PROT_WRITE | PROT_EXEC,
                                     MAP_PRIVATE | MAP_FIXED, provider_fd, 0);
    // printf("sharemalloc_handle_fork\n");
    use_share_malloc = false;
    is_provider_forked = true;
    int dest_fd;
    char buffer[1024 * 1024];
    ssize_t bytes_read;
    assert(MemFileName != NULL);
    // 打开或创建目标文件
    dest_fd = open(MemFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        printf("child open error");
        return;
    }
    fsync(provider_fd);
    lseek(provider_fd, 0, SEEK_SET);

    // 从fd读取数据并写入到目标文件
    while ((bytes_read = read(provider_fd, buffer, 1024 * 1024)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            printf("child write error");
            close(dest_fd);
            return;
        }
    }

    if (bytes_read == -1) {
        printf("child read error");
        close(dest_fd);
        return;
    }
    fsync(dest_fd);
    close(dest_fd);
    dest_fd = open(MemFileName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
    shared_mem = original_mmap(baseaddr, shared_memory_alloc_size,
                               PROT_READ | PROT_WRITE | PROT_EXEC,
                               MAP_PRIVATE | MAP_FIXED, dest_fd, 0);
    is_provider = false; // 之后不需要重新处理
    use_share_malloc = false;
}

// provider 申请内存空间
static void *share_malloc_provier_alloc_memory(unsigned long start_addr,
                                               size_t size) {
    LG("shared_memory_alloc() called with size: %zu\n", size);

    if (!use_share_malloc) {
        LG("!use_share_malloc \n");
        return NULL;
    }
    void *ptr;
    if (ftruncate(provider_fd, size + shared_memory_alloc_size) == -1) {
        LG("ftruncate\n");
        exit(1);
        return NULL;
    }
    void *baseaddr = (void *)(intptr_t)(start_addr + shared_memory_alloc_size);
    printf("baseaddr %p\n", baseaddr);
    fsync(provider_fd);
    ptr = original_mmap(baseaddr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_SHARED | MAP_FIXED, provider_fd,
                        shared_memory_alloc_size);
    if (ptr == MAP_FAILED) {
        printf("mmap %p \n", baseaddr);
        return NULL;
    }
    void *out_addr = ptr;
    shared_memory_alloc_size += size;

    if (ptr == NULL) {
        LG("ptr is null\n");
    }
    return out_addr;
}

// 判断一个指针是不是share_malloc分配的
int my_is_allocated_by_dlmalloc(void *ptr) {
    if (ptr == NULL) {
        return 0;
    }
    // 是provider或者provider的子进程
    if (is_provider || is_provider_forked) {
        if (ptr >= shared_memory_ptr &&
            ptr < shared_memory_ptr + shared_memory_alloc_size) {
            return 1;
        }
        return 0;
    }
    // 是user
    if ((uintptr_t)ptr >= SHARE_MEM_FIX_ADDR_BASE &&
        (uintptr_t)ptr < SHARE_MEM_FIX_ADDR_TOP) {
        return 1;
    }
    return 0;
}
//
pid_t init_pid = -1;
unsigned long start_addr = 0;

void *share_malloc_provier_alloc_memory_with_default_addr(size_t size) {

    assert(start_addr != 0);
    void *ptr = share_malloc_provier_alloc_memory(start_addr, size);
    // printf("123456 %lx %p\n", size, ptr);
    return ptr;
}

void shared_malloc_initialize(struct service_info_t *service_info) {
    start_addr = service_info->start_virtual_addr;
    char *file_path = service_info->shared_state_file_path;
    pthread_mutex_lock(&lock);
    if (share_malloc_initialized == true) {
        LG("share_malloc_initialized\n");
        pthread_mutex_unlock(&lock);
        return;
    }
    void **store_addr = (void **)(intptr_t)(start_addr + 1024);
    share_malloc_provider_init(start_addr, file_path);
    init_pid = getpid();
    LG("pid %d\n", init_pid);
    size_t size = 1024 * 1024 * 8; // 16MB
    void *ptr = share_malloc_provier_alloc_memory(start_addr, size);

    if (!mspace_mallopt(M_GRANULARITY, 1024 * 1024 * 4)) {
        printf("mspace_mallopt error\n");
        exit(0);
    }

    if (!mspace_mallopt(M_MMAP_THRESHOLD, 1024 * 1024 * 1024)) {
        printf("mspace_mallopt error\n");
        exit(0);
    }

    if (!mspace_mallopt(M_TRIM_THRESHOLD, 1024 * 1024)) {
        printf("mspace_mallopt error\n");
        exit(0);
    }

    shared_mspace = create_mspace_with_base(ptr, size, 0);
    *store_addr = shared_mspace;

    LG("initialize_shared_malloc at ptr %p %p\n", ptr, shared_mspace);
    share_malloc_initialized = true;
    pthread_mutex_unlock(&lock);

    service_info->ld_so_hared_map_ptr = (void **)(intptr_t)(start_addr + 4096);

    service_info->pthread_proxy = (void *)(intptr_t)(start_addr + 4096 + 1024);
}

void *share_malloc(size_t size) {
    while (share_malloc_initialized == false) {
        // LG("dasd\n");
    }
    pthread_mutex_lock(&lock);
    void *ptr = mspace_malloc(shared_mspace, size);
    pthread_mutex_unlock(&lock);
    return ptr;
}

void share_free(void *ptr) {
    pthread_mutex_lock(&lock);
    if (my_is_allocated_by_dlmalloc(ptr)) {
        mspace_free(shared_mspace, ptr);
    } else {
        real_free(ptr);
    }
    pthread_mutex_unlock(&lock);
}
static calloc_func_t real_calloc = NULL;
void *share_calloc(size_t nmemb, size_t size) {
    pthread_mutex_lock(&lock);
    void *ptr = mspace_calloc(shared_mspace, nmemb, size);
    pthread_mutex_unlock(&lock);
    return ptr;
}

void share_print_mem_info() { fsync(provider_fd); }

void *share_realloc(void *ptr, size_t size) {

    static realloc_func_t real_realloc_realloc = NULL;
    if (!real_realloc_realloc) {
        real_realloc_realloc = (realloc_func_t)dlsym(RTLD_NEXT, "realloc");
    }

    pthread_mutex_lock(&lock);
    if (size == 0) {
        free(ptr);
    }
    void *new_ptr = NULL;
    if (my_is_allocated_by_dlmalloc(ptr) || ptr == NULL) {
        new_ptr = mspace_realloc(shared_mspace, ptr, size);
        pthread_mutex_unlock(&lock);
    } else {
        pthread_mutex_unlock(&lock);
        new_ptr = share_malloc(size);

        void *old_ptr = real_realloc_realloc(ptr, size);
        memcpy(new_ptr, old_ptr, size);
        free(old_ptr);
    }
    return new_ptr;
}

long id = 0;
void free(void *ptr) {

    if (my_is_allocated_by_dlmalloc(ptr)) {
        share_free(ptr);
    } else {
        real_free(ptr);
    }
}

void *calloc(size_t nmemb, size_t size) {
    if (!real_calloc) {
        real_calloc = (calloc_func_t)dlsym(RTLD_NEXT, "calloc");
    }
    if (is_shared_malloc_used()) {
        void *ptr = share_calloc(nmemb, size);
        return ptr;
    } else {
        void *ptr = real_calloc(nmemb, size);
        return ptr;
    }
}

void *realloc(void *ptr, size_t size) {
    static realloc_func_t real_realloc = NULL;
    if (!real_realloc) {
        real_realloc = (realloc_func_t)dlsym(RTLD_NEXT, "realloc");
    }
    bool is_shared_ptr = my_is_allocated_by_dlmalloc(ptr);
    if (is_shared_ptr) {
        void *new_ptr = share_realloc(ptr, size);
        return new_ptr;
    } else {

        void *new_ptr = real_realloc(ptr, size);
        return new_ptr;
    }
}

void *malloc(size_t size) {
    void *ptr = NULL;
    if (is_shared_malloc_used()) {
        ptr = share_malloc(size);
    } else {
        if (real_malloc == NULL) {
            real_malloc = (malloc_func_t)dlsym(RTLD_NEXT, "malloc");
        }
        ptr = real_malloc(size);
    }
    return ptr;
}

size_t malloc_usable_size(void *ptr) {
    if (is_shared_malloc_used() || my_is_allocated_by_dlmalloc(ptr)) {
        return dlmalloc_usable_size(ptr);
    } else {
        return original_malloc_usable_size(ptr);
    }
}

pid_t fork(void) {
    // TODO: 在这里添加自定义fork行为
    //  LG("Overridden fork() called\n");
    if (is_provider && is_shared_malloc_used()) {
        int pipe_fd[2];
        char buf;

        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid_t pid;
        pid = real_fork();

        if (pid == 0) {
            sharemalloc_handle_fork();
            close(pipe_fd[0]);         // 关闭读取端
            write(pipe_fd[1], "1", 1); // 向管道写入一个字符
            close(pipe_fd[1]);         // 关闭写入端

            return pid;
        }
        close(pipe_fd[1]); // 关闭写入端
        read(pipe_fd[0], &buf,
             1); // 从管道中读取一个字符，这将阻塞父进程直到子进程写入数据
        close(pipe_fd[0]); // 关闭读取端
        printf("forked\n");
        return pid;
    }

    return real_fork();
}