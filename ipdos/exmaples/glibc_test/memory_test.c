#include "ipdos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// #include <time.h>
int main(){
    ipdos_provider_register("memory_test", "1.0.0");
    ipdos_provider_share_malloc_init();
    int num = 1024*512;
    int* record_ptr[100];
    for (int j = 0; j<200; j++){
        int* ptr = (int*) malloc(num*sizeof(int));
        record_ptr[j]= ptr;
        printf("%p\n", ptr);
        for (int i = 0; i< num; i++){
            ptr[i] = i;
        }
    }
    
    
    int sleep_time = 0;
    while(sleep_time > 0){
        sleep(1);
        sleep_time -= 1;
    }
    for (int j = 0; j<50; j++){
        printf("try free %p\n", record_ptr[j*4]);
        free(record_ptr[j*4]);
        free(record_ptr[j*4+2]);
        printf("finish free\n");
    }
    
    printf("free ptr finish \n");
    share_print_mem_info();
    while(1){
        sleep(1);
    }

}