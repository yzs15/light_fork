#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
// #include "jemalloc_conf.h"
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "dlmalloc.h"
#include <errno.h>
#include "ipdos_util/info_util.h"
extern unsigned long service_virtual_addr_start;
extern unsigned long service_virtual_addr_end;
extern char*  service_shared_state_file_path;

// 保留前8个字节用给出一致的数据结构，用于多个进程间的共享。例如可以存储sys.module的指针
#define SHARE_MALLOC_OFFEST 8192
extern  void *shared_memory_ptr;

//设置是否从共享内存加载
bool set_load_from_shared_memory_env(bool env);
//判断是否从共享内存加载
bool is_load_from_shared_memory();

bool is_shared_malloc_used();

bool ipdos_set_mem_file_name(char* file_name);

void shared_malloc_initialize(struct service_info_t *service_info);
void share_malloc_load_builtin_func();