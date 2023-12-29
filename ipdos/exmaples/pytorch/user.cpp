// #include <iostream>
#include <string.h>
extern "C" {
    #include "sharemalloc_dlmalloc.h"
}
struct service_func
{
    char* name;
    void* func;
};

typedef void* (*func_t) (int num);
struct service_func *service_head = (struct service_func *)(uintptr_t)0x7f45000428d0;
int main(){
    load_shared_so();
    struct service_func item;
    func_t f = NULL;
    for (int i=0; i<1; i++){
        item = service_head[i];
        if (strcmp("service_torch_eye", item.name) == 0){
            f = (func_t)item.func;
        }
    }
    if (f == NULL) {
        printf("dlsym failed\n");
        return 0;
    }
    printf("%p\n", f);
    void* tensor = f(3);
}