#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU 
#define __USE_GNU
#endif
#include "ipdos/ipdos.h"
#include "ipdos_util/info_util.h"
#include "ipdos_util/common_util.h"
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
    share_malloc_load_builtin_func();
   
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
