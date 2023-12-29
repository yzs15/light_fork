extern "C" {
    #include "ipdos.h"
}
#include <stdio.h>
#include <dlfcn.h>
// #include <torch/torch.h>
typedef void* (*func_t) (int num);
struct service_func
{
    char* name;
    void* func;
};

struct service_func *service_head = NULL;
int main() {
    ipdos_provider_register("pytorch_service", "0.0.1");
    ipdos_provider_share_malloc_init();
    
    void* s = dlopen("./libservice.so", RTLD_NOW);
   
    if (s == NULL) {
        printf("dlopen failed\n");
    }
   
    func_t f = (func_t)dlsym(s, "service_torch_eye");
     if (f == NULL) {
        printf("dlsym failed\n");
   
        return 0;
    }

    // service_head = (struct service_func *)dlsym(s, "service_head");
    //  if (service_head == NULL) {
    //     printf("dlsym failed\n");
    //     return 0;
    // }

    // struct service_func item;
    // func_t f = NULL;
    // for (int i=0; i<1; i++){
    //     item = service_head[i];
    //     if (strcmp("service_torch_eye", item.name) == 0){
    //         f = (func_t)item.func;
    //     }
    // }
    // if (f == NULL) {
    //     printf("dlsym failed\n");
    //     return 0;
    // }

    // void* tensor = f(3);
    // printf("%p %p\n", service_head, f);
    // while(1){
    //     sleep(1);
    // }
    // std::cout << tensor << std::endl;
}