#include <stdbool.h>
#include <stddef.h>
struct ipdos_service_info_t{
    char* key;
    void* addr;
    void* end_addr;
    long int shared_so_nsid;
};

void ipdos_provider_register(char* service_name, char* version);
void ipdos_provider_share_malloc_init();

void ipdos_user_init();
struct ipdos_service_info_t* ipdos_user_load_service(char* service_name, char* version);
struct service_info_t* ipdos_service_info_get_by_addr(void* addr);
void ipdos_close();
void ipdos_open();



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


extern void yuzishu_provider_set_shared_map_ptr(void* ptr);
extern void yuzishu_user_add_service(void* so_list_head, long int shared_so_nsid, unsigned long start_addr, unsigned long end_addr);
extern long int dl_get_new_namespace(void);

// bool ipdos_is_main_thread();
bool ipdos_util_is_user();
bool ipdos_util_is_provider();

typedef void (*pthread_proxy_recover_func_t)(void *);
void pthread_proxy_provider_add_recover_function(pthread_proxy_recover_func_t f, void* args);

void pthread_proxy_user_set_addr(void* addr);

void pthread_proxy_user_unset_addr();


bool ipdos_set_loading(char* key);

bool ipdos_unset_loading(void);

bool ipdos_is_loading(char ** key);