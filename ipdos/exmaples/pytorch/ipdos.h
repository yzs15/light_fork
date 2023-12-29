/* provider 执行流程
    调用ipdos_init初始化，mmap占用SHARE_MEM_FIX_ADDR_BASE ~ SHARE_MEM_FIX_ADDR_TOP的虚拟地址空间
    调用ipdos_provider_register，获取service的虚拟地址空间
    调用ipdos_provider_share_malloc_init，初始化share malloc
*/

void ipdos_provider_register(char* service_name, char* version);
void ipdos_provider_share_malloc_init();

void ipdos_user_init();
void ipdos_user_load_service(char* service_name, char* version);

void *share_malloc(size_t size);
void  share_free(void *ptr);
void *share_calloc(size_t nmemb, size_t size);
void *share_realloc(void *ptr, size_t size);
void sharemalloc_handle_fork(void);
int my_is_allocated_by_dlmalloc(void* ptr);

bool is_load_from_shared_memory();
bool set_load_from_shared_memory_env(bool env);
extern  void *shared_memory_ptr;
bool is_shared_malloc_used();