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
extern void *(*original_mmap)(void *addr, size_t length, int prot, int flags, int fd,
                       off_t offset);
void ipdos_init(){
    share_malloc_load_builtin_func();
    original_mmap = dlsym(RTLD_NEXT, "mmap");
    void* baseaddr = original_mmap(NULL, SHARE_MEM_FIX_ADDR_SIZE, PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    if (baseaddr == MAP_FAILED) {
        printf("ipdos_init mmap failed\n");
        return;
    }
    provider_service_info = malloc(sizeof(struct service_info_t));
    provider_service_info->start_virtual_addr = (uintptr_t)baseaddr;
    provider_service_info->end_virtual_addr = (uintptr_t)baseaddr + 1024*1024*1024;
    provider_service_info->ld_so_hared_map_ptr = NULL;
    provider_service_info->pthread_proxy = NULL;
    provider_service_info->service_name = "ipdos";
    provider_service_info->version = "0.0.1";
    provider_service_info->shared_so_nsid = 0;
    char* fileName = malloc(FILE_NAME_SIZE);
    // snprintf(fileName, FILE_NAME_SIZE, "/dev/shm/memory_file_%ld", get_time_us());
    
    //环境变量设置共享内存文件名
    char* env_file_name = getenv("IPDOS_SHARED_MEM_FILE");
    if(env_file_name != NULL){
        snprintf(fileName, FILE_NAME_SIZE, "%s", env_file_name);
    }else{
        //error
        printf("IPDOS_SHARED_MEM_FILE not set\n");
        exit(1);
    }
    provider_service_info->shared_state_file_path = fileName;
    shared_malloc_initialize(provider_service_info);
   

}  
