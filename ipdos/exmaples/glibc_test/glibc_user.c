#include "ipdos.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
typedef int (*func_t)(void);
void load(char* name, char* version, long int addr){
    ipdos_user_load_service(name, version);
    printf("-------------------main start-----------------------\n");
    void *s = (void *)(uintptr_t) addr;
    func_t f = dlsym(s, "share_lib_func");
    // int* variable =  dlsym(s, "variable");
    if (f == NULL) {
        printf("dlsym failed\n");
    }
    
    // int ret = f();

    // func_t p = dlsym(s, "print_info");
    // if (f == NULL) {
    //     printf("dlsym failed\n");
    // }
    // printf("s %p,  share_lib_func %p print_info %p\n",s,f, p);
    printf("----------------------------------------------------------\n");
    int ret = f();

    printf("s %p, func_t %p ret = %d\n",s, f,  ret);
    // ret = f();
    // printf("s %p, func_t %p ret = %d variable %p %d\n",s, f,  ret, variable, *variable);
}

int main(void){
    ipdos_open();
    ipdos_user_init();
    // load("libsharetest", "1.0.0", 0x6008000023e0);
    load("libsharetest", "2.0.0", 0x6000000023e0);
    // void* s = dlopen("libsharetestfather.so", RTLD_NOW|RTLD_GLOBAL);
    // printf("s %p\n", s);
    // func_t f = dlsym(s, "print_info");
    // if (f == NULL) {
    //     printf("dlsym failed\n");
    // }

    // int ret = f();

    ipdos_close();
}