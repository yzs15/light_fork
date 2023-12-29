#include<iostream>
#include <dlfcn.h>
#include <mutex>
thread_local int father_var = 1000;

typedef int (*func_t)(void);

extern "C" {

int print_info(void){
    printf("std::call_once %p\n", std::__once_proxy);
    return 0;
}

// int share_lib_func(void);
int father_share_lib_func(void)
{
    // share_lib_func();
    return 0;
}
}