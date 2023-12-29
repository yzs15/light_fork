// #include "ipdos_util/hashmap.h"
// #include "ipdos_util/list.h"
// #include <pthread.h>

// struct pthread_proxy_t
// {
//     /* data */
//     struct hashmap_s hashmap;
//     struct hashmap_s user_hashmap;
//     struct list_head_t recover_func_list;
//     pthread_mutex_t mutex;
// };

// int pthread_proxy_init(struct pthread_proxy_t *pthread_proxy, const unsigned initial_size);
// void pthread_proxy_user_init();
// void pthread_proxy_user_recover(struct pthread_proxy_t *pthread_proxy);


// typedef void (*pthread_proxy_recover_func_t)(void *);
// void pthread_proxy_provider_add_recover_function(pthread_proxy_recover_func_t f, void* args);

// void pthread_proxy_user_run_recover_func(struct pthread_proxy_t *pthread_proxy, unsigned long addr);