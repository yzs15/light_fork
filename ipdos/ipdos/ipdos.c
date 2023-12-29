#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU 
#define __USE_GNU
#endif
#include "ipdos/ipdos.h"
#include "ipdos/ipdos_client.h"
#include "ipdos_util/info_util.h"
#include "ipdos_util/common_util.h"
#include "ipdos/pthread_proxy.h"
#include "sharemalloc_dlmalloc.h"
#include <dlfcn.h>
#include <sys/mman.h>
#include <assert.h>

/* provider 执行流程
    调用ipdos_init初始化，mmap占用SHARE_MEM_FIX_ADDR_BASE ~ SHARE_MEM_FIX_ADDR_TOP的虚拟地址空间
    调用ipdos_provider_register，获取service的虚拟地址空间
    调用ipdos_provider_share_malloc_init，初始化share malloc

*/


#define MAP_ANONYMOUS 0x20
char* IPDOS_MANAGER_ADDR = NULL;
// #define IPDOS_MANAGER_ADDR "tcp://127.0.0.1:5555"
// #define IPDOS_MANAGER_ADDR "tcp://ipdos.domain:5555"
#define SHARE_FILE_FOLDER "/home/yuzishu/share_folder/"
// #define SHARE_FILE_FOLDER "/dev/shm/lass_ict/"
#define IPDOS_MANAGER_LOG_PATH "/dev/shm/lass_ict/ipdos_manager_log"
void ipdos_init() __attribute__((constructor));


struct ipdos_client_t* ipdos_client=NULL;
struct service_info_t* provider_service_info=NULL;
struct service_map_t user_service_map;

#include <time.h>
long get_time_us(){
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    return timestamp.tv_sec *1000*1000 + timestamp.tv_nsec /1000;
}

#define FILE_NAME_SIZE 150

bool ipdos_inited = false;
//初始化：mmap占用SHARE_MEM_FIX_ADDR_BASE ~ SHARE_MEM_FIX_ADDR_TOP的虚拟地址空间
void ipdos_init(){
    // if (ipdos_inited) {
    //     return;
    // }
    // printf("ipdos_init\n");
    share_malloc_load_builtin_func();
   
    // LG("ipdos_init\n");
    void* baseaddr = (void*)(uintptr_t)SHARE_MEM_FIX_ADDR_BASE;
    void* addr = mmap(baseaddr, SHARE_MEM_FIX_ADDR_SIZE, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1,0);
    provider_service_info = malloc(sizeof(struct service_info_t));
    provider_service_info->start_virtual_addr = (uintptr_t)baseaddr;
    provider_service_info->end_virtual_addr = (uintptr_t)baseaddr + 1024*1024*1024;
    provider_service_info->ld_so_hared_map_ptr = NULL;
    provider_service_info->pthread_proxy = NULL;
    provider_service_info->service_name = "ipdos";
    provider_service_info->version = "0.0.1";
    provider_service_info->shared_so_nsid = 0;
    char* fileName = malloc(FILE_NAME_SIZE);
    snprintf(fileName, FILE_NAME_SIZE, "/dev/shm/memory_file_%ld", get_time_us());
    provider_service_info->shared_state_file_path = fileName;

    shared_malloc_initialize(provider_service_info);
   

}  

// void ipdos_open(){
//     // ipdos_client = ipdos_client_create(IPDOS_MANAGER_ADDR);
// }
// void ipdos_close(){
//     // ipdos_client_destroy(ipdos_client);
// }

// //provider 需要首先调用ipdos_provider_register注册服务，获取逻辑虚拟地址
// void ipdos_provider_register(char* service_name, char* version){
//     ipdos_util_set_identity(IPDOS_PROVIDER);
//     share_malloc_load_builtin_func();
//     char* file_path = malloc(strlen(SHARE_FILE_FOLDER)+strlen(service_name)+strlen(version)+1);
//     strcpy(file_path, SHARE_FILE_FOLDER);
//     strcat(file_path, service_name);
//     strcat(file_path, version);
//     provider_service_info = ipdos_client_alloc_addr(IPDOS_MANAGER_ADDR, service_name, version, file_path);
//     free(file_path);
//     // service_info_print(provider_service_info);
// }


// void ipdos_provider_share_malloc_init(){
//     assert(provider_service_info != NULL);
//     shared_malloc_initialize(provider_service_info);
//     yuzishu_provider_set_shared_map_ptr(provider_service_info->ld_so_hared_map_ptr);
//     struct pthread_proxy_t* pthread_proxy = provider_service_info->pthread_proxy;
//     LG("%p %p", pthread_proxy, &(pthread_proxy->hashmap));
//     pthread_proxy_init(provider_service_info->pthread_proxy, 10);
// }

// //user 需要调用ipdos_user_init初始化，才可以进行后续操作
// bool ipdos_user_inited = false;
// void ipdos_user_init(){
//     if (ipdos_user_inited) {
//         return;
//     }
//     ipdos_user_inited = true;
//     ipdos_util_set_identity(IPDOS_USER);
//     service_info_map_init(&user_service_map, 10);
//     //传递user_service_map的地址给share malloc
//     shared_malloc_set_user_service_map(&user_service_map);
//     service_info_set_user_service_map(&user_service_map);
//     set_load_from_shared_memory_env(true);
// }


// // 从manager获取服务信息
// void ipdos_user_get_service_info_from_manager(char* service_name, char* version){
//     // struct service_info_t* info= ipdos_client_query(ipdos_client, service_name, version);
//     struct service_info_t* info= ipdos_query_from_file(IPDOS_MANAGER_LOG_PATH, service_name, version);
//     service_info_put_no_force(&user_service_map, info);
//     service_info_free(info);
// }



// //返回service的key
// struct ipdos_service_info_t* ipdos_user_load_service(char* service_name, char* version){
//     ipdos_init();
//     ipdos_user_init();
//     // printf("ipdos_user_load_service %s %s\n", service_name, version);
//     // long start = get_time_us();
//     ipdos_user_get_service_info_from_manager(service_name, version);
//     // long end = get_time_us();
//     // printf(" ipdos_user_load_service net %ld\n", end-start);
//     struct service_info_t* info = service_info_get(&user_service_map, service_name, version);
//     assert(info != NULL);
//     long int nsid = dl_get_new_namespace();

//     share_malloc_user_load_service(info, nsid);
//     yuzishu_user_add_service(info->ld_so_hared_map_ptr, nsid, info->start_virtual_addr, info->end_virtual_addr);
//     load_shared_so(info, nsid);
//     struct pthread_proxy_t* pthread_proxy = info->pthread_proxy;
//     LG("%p %p", pthread_proxy, &(pthread_proxy->hashmap));
//     pthread_proxy_user_recover(info->pthread_proxy);
//     pthread_proxy_user_run_recover_func(info->pthread_proxy, info->start_virtual_addr);

//     size_t key_len;
//     char *key = service_info_key_gen(service_name, version, &key_len);
//     struct ipdos_service_info_t* ipdos_service_info = malloc(sizeof(struct ipdos_service_info_t));
//     ipdos_service_info->key = key;
//     ipdos_service_info->addr = (void*)(uintptr_t)info->start_virtual_addr;
//     ipdos_service_info->end_addr = (void*)(uintptr_t)info->end_virtual_addr;
//     ipdos_service_info->shared_so_nsid = nsid;
//     return ipdos_service_info;
// }   

// char *ipdos_get_service_key_by_addr(void* addr){
//     return NULL;
// }

// struct service_info_t* ipdos_service_info_get_by_addr(void* addr){
//     struct service_info_t* info = service_info_get_by_addr(&user_service_map, addr);
//     return info;
//     // struct ipdos_service_info_t* ipdos_service_info = malloc(sizeof(struct ipdos_service_info_t));
//     // size_t key_len;
//     // ipdos_service_info->key = service_info_key_gen(info->service_name, info->version, &key_len);
//     // ipdos_service_info->addr = (void*)(uintptr_t)info->start_virtual_addr;
//     // ipdos_service_info->end_addr = (void*)(uintptr_t)info->end_virtual_addr;
//     // ipdos_service_info->shared_so_nsid = info->shared_so_nsid;
//     // return ipdos_service_info;
// }


// char* ipdos_set_loading_key=NULL;

// bool ipdos_set_loading(char* key){
//     //TODO 加锁
//     ipdos_set_loading_key = key;
//     return true;
// }

// bool ipdos_unset_loading(void){
//     free(ipdos_set_loading_key);
//     ipdos_set_loading_key = NULL;
//     return true;
// }

// bool ipdos_is_loading(char ** key){
//     if (ipdos_set_loading_key == NULL){
//         *key = NULL;
//         return false;
//     }else{
//         *key = ipdos_set_loading_key;
//         return true;
//     }

// }

