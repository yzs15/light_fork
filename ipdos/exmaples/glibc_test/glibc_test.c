#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <malloc.h>
#include <sys/mman.h>
#include <unistd.h>
#include <link.h>
#include "ipdos.h"
#include <string.h>
typedef int (*func_t)(void);
int hahhah = 1;
// extern void* memalign(size_t alignment, size_t size);
extern void yuzishu_set_pointer_test (void* ptr);
void* s =NULL;
void* test_dl_open(char* path){
    // Lmid_t nsid = dl_get_new_namespace();
    // s = dlmopen(nsid, path, RTLD_NOW);
     s = dlopen(path, RTLD_NOW);
     printf("s %p\n", s);
    if (s == NULL) {
        char * err = dlerror();
        printf("dlopen failed %s\n", err);
        // printf("dlopen failed\n");
    }
    func_t f = dlsym(s, "share_lib_func");
    // int* variable =  dlsym(s, "variable");
    // if (f == NULL) {
    //     printf("dlsym failed\n");
    // }

    // printf("s %p, func_t %p\n",s, f);
    printf("---------------------------------------\n");
    int ret = f();
    // func_t p  = dlsym(s, "print_info");
    // if (p == NULL) {
    //     printf("dlsym failed\n");
    // }
    printf("s %p,  share_lib_func %p \n",s,f);
    // ret = p();
    // printf("s %p, func_t %p ret = %d variable %p %d\n",s, f,  ret, variable, *variable);
    // ret = f();
    // printf("s %p, func_t %p ret = %d variable %p %d\n",s, f,  ret, variable, *variable);
    // printf("hahhah %p %d\n", &hahhah, hahhah);

    // s = dlmopen(nsid, "./libsharetestfather.so", RTLD_NOW);
    // s = dlopen("./libsharetestfather.so", RTLD_NOW);
    // if (s == NULL) {
    //     char * err = dlerror();
    //     printf("dlopen failed %s\n", err);
    //     // printf("dlopen failed\n");
    // }
    // f = dlsym(s, "father_share_lib_func");
    // f();
   return s;
}

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
    printf("s %p,  share_lib_func %p \n",s,f);
    func_t p = dlsym(s, "print_info");
    if (p == NULL) {
        printf("dlsym failed\n");
    }
    // printf("s %p,  share_lib_func %p print_info %p\n",s,f, p);
    printf("----------------------------------------------------------\n");
    // int ret = f();

    // printf("s %p, func_t %p ret = %d\n",s, f,  ret);
    p();
    // ret = f();
    // printf("s %p, func_t %p ret = %d variable %p %d\n",s, f,  ret, variable, *variable);
}



int main(int argc, char* argv[]){
    if (strcmp(argv[1],"provider")==0){
        ipdos_open();
        ipdos_provider_register("libsharetest", argv[2]);
        ipdos_provider_share_malloc_init();
        void*s = test_dl_open("./libsharetest.so");
        int i = 15;
        while (i>0){
            sleep(1);
        }
    }

    if (strcmp(argv[1],"user")==0){
        dlopen("libsharetestfather.so", RTLD_NOW);
        printf("%p, dlopen \n", dlopen);
        ipdos_open();
        ipdos_user_init();
        load("libsharetest", "2.0.0", 0x60000000b520);
        ipdos_close();
    }
    
    
   
}

