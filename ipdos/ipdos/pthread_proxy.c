// #ifndef _GNU_SOURCE
// #define _GNU_SOURCE
// #endif
// #ifndef __USE_GNU 
// #define __USE_GNU
// #endif
// #include <dlfcn.h>
// #include <pthread.h>
// #include "ipdos_util/hashmap.h"
// #include "ipdos/pthread_proxy.h"
// #include "ipdos_util/common_util.h"
// #include "ipdos_util/info_util.h"
// #include <assert.h>
// #include <stdio.h>
// #include "ipdos_util/debug.h"
// struct pthread_proxy_t *pthread_proxy_copy;
// void* pthread_proxy_user_addr_info = NULL;
// pthread_mutex_t *pthread_proxy_user_addr_info_mutex = NULL;

// void pthread_proxy_user_init(){
//     pthread_mutexattr_t mutexAttr;
//     pthread_mutexattr_init(&mutexAttr);
//     pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE); 
//     pthread_proxy_user_addr_info_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
//     assert(pthread_mutex_init(pthread_proxy_user_addr_info_mutex, &mutexAttr) ==0);
    
// }

// int pthread_proxy_init(struct pthread_proxy_t *pthread_proxy, const unsigned initial_size){
//     if (0 != hashmap_create(initial_size, &(pthread_proxy->hashmap))) {
//       return 0;
//     }
//     if (0 != hashmap_create(initial_size, &(pthread_proxy->user_hashmap))) {
//       return 0;
//     }

//     list_init(&(pthread_proxy->recover_func_list));
//     pthread_proxy_copy = pthread_proxy;
//     return 1;
// }

// struct pthread_proxy_record_info_t {
//     pthread_key_t *old_key_addr;
//     pthread_key_t old_key;
//     pthread_key_t new_key;
//     void (*__destr_function) (void *);
//     const void* value;
// };

// void pthread_proxy_provider_record_create(pthread_key_t *__key, void (*__destr_function) (void *)){
//     LG("key=%d key_addr=%p pointer=%p\n", *__key, __key, __destr_function);
//     void* element = hashmap_get(&(pthread_proxy_copy->hashmap), __key, sizeof(pthread_key_t));
//     if (element != NULL){
//         struct pthread_proxy_record_info_t* record_info = (struct pthread_proxy_record_info_t*) element;
//         record_info->__destr_function = __destr_function;
//         record_info->old_key_addr = __key;
//         record_info->value = NULL;
//         record_info->old_key = *__key;
//         return;
//     }

//     struct pthread_proxy_record_info_t* record_info = malloc(sizeof(struct pthread_proxy_record_info_t));
//     record_info->old_key_addr = __key;
//     record_info->__destr_function = __destr_function;
//     record_info->value = NULL;
//     record_info->old_key = *__key;
//     //不需要锁，因为只记录主线程的信息
//     // pthread_mutex_lock(&(pthread_proxy_copy->mutex));
//     hashmap_put(&(pthread_proxy_copy->hashmap), __key, sizeof(pthread_key_t), record_info);
//     // pthread_mutex_unlock(&(pthread_proxy_copy->mutex));
// }   

// void pthread_proxy_provider_record_setspecific(pthread_key_t __key, const void *__pointer){
//     LG("key=%d pointer=%p\n", __key, __pointer);
//     void* element = hashmap_get(&(pthread_proxy_copy->hashmap), &__key, sizeof(pthread_key_t));
//     // assert(element != NULL);
//      if (element == NULL){
//         return;
//     }
//     struct pthread_proxy_record_info_t* record_info = (struct pthread_proxy_record_info_t*) element;
//     record_info->value = __pointer;
//     return;
// }

// void pthread_proxy_provider_record_delete(pthread_key_t __key){
//     LG("key=%d\n", __key);
//     void* element = hashmap_get(&(pthread_proxy_copy->hashmap), &__key, sizeof(pthread_key_t));
//     if (element == NULL){
//         return;
//     }
//     // assert(element != NULL);
//     free(element);
//     hashmap_remove(&(pthread_proxy_copy->hashmap), &__key, sizeof(pthread_key_t));
//     return;
// }

// int (*original_pthread_key_create) (pthread_key_t *__key, void (*__destr_function) (void *)) = NULL;
// int (*original_pthread_key_delete) (pthread_key_t __key) = NULL;
// int (*original_pthread_setspecific) (pthread_key_t __key, const void *__pointer) = NULL;
// void* (*original_pthread_getspecific) (pthread_key_t __key) = NULL;

// static int pthread_proxy_user_recover_iterate(void* const context, void* const value) {
//     if (original_pthread_setspecific == NULL) {
//         original_pthread_setspecific = dlsym(RTLD_NEXT, "pthread_setspecific");
//     }
//     if (original_pthread_key_create == NULL) {
//         original_pthread_key_create = dlsym(RTLD_NEXT, "pthread_key_create");
//     }

//     LG(" context %p value %p\n", context, value);
//     struct pthread_proxy_record_info_t* info = ( struct pthread_proxy_record_info_t*) value;
//     original_pthread_key_create(&(info->new_key), info->__destr_function);
//     if (info->value != NULL){
//         original_pthread_setspecific(info->new_key, info->value);
//     }
//     return 1;
// }


// // struct pthread_proxy_t *user_pthread_proxy_test = NULL;
// void pthread_proxy_user_recover(struct pthread_proxy_t *pthread_proxy){
//     // pthread_mutex_lock(&(pthread_proxy->mutex));
//     hashmap_iterate(&(pthread_proxy->hashmap), pthread_proxy_user_recover_iterate, NULL);
//     // pthread_proxy_user_run_recover_func(pthread_proxy);
//     // user_pthread_proxy_test = pthread_proxy;
//     // pthread_mutex_unlock(&(pthread_proxy->mutex));
// }


// int pthread_proxy_user_key_delete (struct pthread_proxy_t *pthread_proxy, pthread_key_t __key, bool *ok){
    
//     void* element = hashmap_get(&(pthread_proxy->hashmap), &__key, sizeof(pthread_key_t));
//     if (element == NULL){
//         *ok = false;
//         return -1;
//     }
//     *ok = true;
//     struct pthread_proxy_record_info_t* record_info = (struct pthread_proxy_record_info_t*) element;
//     int res = original_pthread_key_delete(record_info->new_key);
//     hashmap_remove(&(pthread_proxy->hashmap), &__key, sizeof(pthread_key_t));
//     return res;
// }

// int pthread_proxy_user_setspecific (struct pthread_proxy_t *pthread_proxy, pthread_key_t __key, const void *__pointer, bool *ok){
//     void* element = hashmap_get(&(pthread_proxy->hashmap), &__key, sizeof(pthread_key_t));
//     if (element == NULL){
//         *ok = false;
//         return -1;
//     }

//     *ok = true;
//     struct pthread_proxy_record_info_t* record_info = (struct pthread_proxy_record_info_t*) element;
//     return original_pthread_setspecific(record_info->new_key, __pointer);

// }

// void * pthread_proxy_user_getspecific (struct pthread_proxy_t *pthread_proxy, pthread_key_t __key, bool *ok){
//     LG("pthread_proxy_user_getspecific key = %d\n", __key);
//     void* element = hashmap_get(&(pthread_proxy->hashmap), &__key, sizeof(pthread_key_t));
//     LG("pthread_proxy_user_getspecific element = %p\n", element);
//     if (element == NULL){
//         *ok = false;
//         return NULL;
//     }
//     *ok = true;
    
//     struct pthread_proxy_record_info_t* record_info = (struct pthread_proxy_record_info_t*) element;
//     LG("pthread_proxy_user_getspecific keyin %d key %d  key_addr %p new_key %d \n", __key, record_info->old_key, record_info->old_key_addr, record_info->new_key);
//     return original_pthread_getspecific(record_info->new_key);
// }



// int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *)){
//     LG("pthread_key_create\n");
//     if (original_pthread_key_create == NULL) {
//         original_pthread_key_create = dlsym(RTLD_NEXT, "pthread_key_create");
//     }

//     int res = 0;

//     res = original_pthread_key_create(__key, __destr_function);
    
//     if (ipdos_is_main_thread() && ipdos_util_is_provider() && res == 0 && ipdos_util_is_shared_ptr(__key)){
//         pthread_proxy_provider_record_create(__key, __destr_function);
//     }
//     LG("pthread_key_create %p %d  %p success = %d\n", __key, *__key, __destr_function, res);
//     return res;

// }

// int pthread_key_delete (pthread_key_t __key){
//     LG("pthread_key_delete\n");
//     if (original_pthread_key_delete == NULL) {
//         original_pthread_key_delete = dlsym(RTLD_NEXT, "pthread_key_delete");
//     }
//      int res =0;
//     if (ipdos_util_is_user()){
//         pthread_mutex_lock(pthread_proxy_user_addr_info_mutex);
//         bool ok = false;
//         void* addr;
//         if (pthread_proxy_user_addr_info != NULL){
//             addr = pthread_proxy_user_addr_info;
//         }else{
//             addr = __builtin_return_address(0);
//         }
        
//         LG("dlopen addr %p\n", addr);
//         struct service_info_t* info = service_info_get_by_addr(service_info_get_user_service_map(),  addr);
//         if (info != NULL){
//              res = pthread_proxy_user_key_delete(info->pthread_proxy, __key, &ok);
//             if (ok){
//                 pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//                 return res;
//             }
//         }
       
//         res = original_pthread_key_delete(__key);
//         pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//         return res;
//     }else{
//         res = original_pthread_key_delete(__key);
//     }
//     if (ipdos_is_main_thread() && ipdos_util_is_provider() && res == 0){
//         pthread_proxy_provider_record_delete(__key);
//     }
//     return res;
// }

// int pthread_setspecific (pthread_key_t __key, const void *__pointer){
//     if (original_pthread_setspecific == NULL) {
//         original_pthread_setspecific = dlsym(RTLD_NEXT, "pthread_setspecific");
//     }
//     if (ipdos_util_is_user()){
//         pthread_mutex_lock(pthread_proxy_user_addr_info_mutex);
//         bool ok = false;
//         void* addr;
//         if (pthread_proxy_user_addr_info != NULL){
//             addr = pthread_proxy_user_addr_info;
//         }else{
//             //TODO default选项，有可能会出错
//             addr = __builtin_return_address(0);
//         }
//         LG("__builtin_return_address %p key= %d\n", addr, __key);
//         struct service_info_t* info = service_info_get_by_addr(service_info_get_user_service_map(),  addr);
//         if (info != NULL){
//             int res = pthread_proxy_user_setspecific(info->pthread_proxy, __key, __pointer, &ok);
//             if (ok){
//                 pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//                 return res;
//             }
//         }
//         pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//         return original_pthread_setspecific(__key, __pointer);
//     }
    
//     int res = original_pthread_setspecific(__key, __pointer);
//     if (ipdos_is_main_thread() && ipdos_util_is_provider() && res == 0){
//         pthread_proxy_provider_record_setspecific(__key, __pointer);
//     }
//     void* addr = __builtin_return_address(0); 
//     LG("pthread_setspecific %d %p success = %d return addr %p threadid %ld is_main_thread %d \n", __key, __pointer, res, addr, pthread_self(), ipdos_is_main_thread());
//     return res;
// }

// #include <execinfo.h>
// #include <stdio.h>
// #include <stdlib.h>

// void print_trace(void) {
//     void *buffer[10];
//     int nptrs = backtrace(buffer, 10);
//     char **strings = backtrace_symbols(buffer, nptrs);
//     if (strings == NULL) {
//         perror("backtrace_symbols");
//         exit(EXIT_FAILURE);
//     }
//     for (int j = 0; j < nptrs; j++) {
//         printf("%s\n", strings[j]);
//     }
//     free(strings);
// }


// void * pthread_getspecific (pthread_key_t __key){
//     // LG_ENV(LG("1\n"); print_trace());
//     LG("pthread_getspecific key = %d is main thread %d  \n", __key, ipdos_is_main_thread());
//     if (original_pthread_getspecific == NULL) {
//         original_pthread_getspecific = dlsym(RTLD_NEXT, "pthread_getspecific");
//     }
//     if (ipdos_util_is_user() ){
//         pthread_mutex_lock(pthread_proxy_user_addr_info_mutex);
//         bool ok = false;
//         void* addr;
//         if (pthread_proxy_user_addr_info != NULL){
//             addr = pthread_proxy_user_addr_info;
//         }else{
//             //TODO default选项，有可能会出错
//             addr = __builtin_return_address(0);
//         }
//         LG("addr %p", addr);
//         struct service_info_t* info = service_info_get_by_addr(service_info_get_user_service_map(),  addr);
//         if (info != NULL){
//             void * res = pthread_proxy_user_getspecific(info->pthread_proxy, __key, &ok);
//             if (ok){
//                 pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//                 return res;
//             }
//         }
//         pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
//         LG("addr %p", addr);
//         return original_pthread_getspecific(__key);
//     }else{
//         void* addr = __builtin_return_address(0);
//         LG("return addr = %p\n", addr);
//     }
//     void * res = original_pthread_getspecific(__key);
//     LG("res %p\n", res);
//     //根据地址，获取service的信息
//     return res;
// }

// struct pthread_proxy_recover_function_item_t{
//      pthread_proxy_recover_func_t f;
//      void* args;
// };

// void pthread_proxy_provider_add_recover_function( pthread_proxy_recover_func_t f, void* args){
//     struct pthread_proxy_recover_function_item_t *item = (struct pthread_proxy_recover_function_item_t*) malloc(sizeof(struct pthread_proxy_recover_function_item_t));
//     item->f = f;
//     item->args = args;
//     list_add( &pthread_proxy_copy->recover_func_list, item);
// }


// void pthread_proxy_user_recover_map(void* data){
//      struct pthread_proxy_recover_function_item_t *item =  (struct pthread_proxy_recover_function_item_t *) data;
//      item->f(item->args);
// }

// // struct user_global_pthread_proxy_t{
// //     struct pthread_proxy_t *pthread_proxy;
// //     struct 
// // };

// // struct user_global_pthread_proxy_t* user_global_pthread_proxy;

// // void pthread_proxy_set_user_global_pthread_proxy(struct pthread_proxy_t *pthread_proxy){
// //     user_global_pthread_proxy = pthread_proxy;
// // }

// // void pthread_proxy_set_user_global_pthread_proxy(struct pthread_proxy_t *pthread_proxy){
// //     user_global_pthread_proxy = pthread_proxy;
// // }


// //设置地址信息，用于user查询到底是哪一个pthread_proxy
// void pthread_proxy_user_set_addr(void* addr){
//     pthread_mutex_lock(pthread_proxy_user_addr_info_mutex);
//     if (pthread_proxy_user_addr_info == NULL){
//         pthread_proxy_user_addr_info = addr;
//     }
   
// }

// void pthread_proxy_user_unset_addr(){
//     pthread_proxy_user_addr_info = NULL;
//     pthread_mutex_unlock(pthread_proxy_user_addr_info_mutex);
// }

// //先运行pthread_setspecific相关的代码，在运行下列代码
// void pthread_proxy_user_run_recover_func(struct pthread_proxy_t *pthread_proxy, unsigned long addr){
//     LG("1 addr = %lx", addr);
//     pthread_proxy_user_set_addr((void*)(uintptr_t) addr);
//     list_map(&pthread_proxy->recover_func_list, pthread_proxy_user_recover_map);
//     pthread_proxy_user_unset_addr();
// };
