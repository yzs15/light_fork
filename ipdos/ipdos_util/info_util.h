#ifndef _INFO_UTIL_H_
#define _INFO_UTIL_H_
#include "hashmap.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#define SHARE_MEM_FIX_ADDR_BASE 0x600000000000
#define SHARE_MEM_FIX_ADDR_TOP  0x7f0000000000
#define SHARE_MEM_FIX_ADDR_SIZE (SHARE_MEM_FIX_ADDR_TOP - SHARE_MEM_FIX_ADDR_BASE)
struct service_info_t
{
    char* service_name;
    char* version;
    char* shared_state_file_path;
    unsigned long start_virtual_addr;
    unsigned long end_virtual_addr;

    //private
    void* share_mmap_list_head;
    void* dl_open_list_head;
    void* shared_mspace;
    void* ld_so_hared_map_ptr;//provider的so_list_head的数据存储的地址

    void* pthread_proxy;
    
    bool shared_so_loaded; //是否已经加载了share so
    long int shared_so_nsid; //share so的nsid

};

struct service_map_t{
    struct hashmap_s hashmap;
};

int service_info_map_init(struct service_map_t *service_map, const unsigned initial_size);

bool service_info_set_user_service_map(struct service_map_t *service_map);
struct service_map_t * service_info_get_user_service_map();


char* service_info_key_gen(char* service_name, char* version, size_t* key_len);

//1 success; 0 failed
int service_info_put_force(struct service_map_t *service_map, struct service_info_t* service_info);

//if not exist then try put; else do nothing;
//return 1 when success
int service_info_put_no_force(struct service_map_t *service_map, struct service_info_t* service_info);

//NULL for fail
struct service_info_t* service_info_get(struct service_map_t *service_map, char* service_name, char* version);

//1 success; 0 failed
int service_info_del(struct service_map_t *service_map, char* service_name, char* version);

void service_info_print( struct service_info_t* service_info);

struct service_info_t* service_info_copy(struct service_info_t* service_info);
void service_info_free(struct service_info_t* service_info);
struct service_info_t* service_info_get_by_addr(struct service_map_t *service_map, void* addr);
#endif