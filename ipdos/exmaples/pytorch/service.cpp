#include <torch/torch.h>
#include <iostream>

extern "C" {
struct service_func
{
    char* name;
    void* func;
   

};




 
// void init(){
//     head = malloc(sizeof(struct service_func ));
//     head->next = NULL;
//     struct service_func * torch_eye = malloc(sizeof(struct service_func ));
//     torch_eye->name 
// }

void * service_torch_eye(int num) {
    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;
    // printf("num = %d\n", num);
    return NULL;
}
struct service_func service_head[]={
    {"service_torch_eye", (void*)service_torch_eye},
};
}