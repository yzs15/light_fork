#include<iostream>
#include <mutex>
#include <dlfcn.h>
#include <string.h>
#include <assert.h>
thread_local int variable = 10;
extern "C" {
extern int hahhah;
pthread_key_t key;
int print_info(void){
    static std::once_flag flag;
    std::call_once(flag, []() {
         printf("call_once\n");
    });
    //  printf("call_once\n");
   

    char *data = (char*)malloc(100);
    strcpy(data, "hello world");
    // 创建键
    // pthread_key_create(&key, NULL);

    // 在此线程中为该键设置一个值
    printf("%s\n", (char*)pthread_getspecific(key));
    assert(pthread_setspecific(key, (void *)data) == 0);
    printf("%s\n", (char*)pthread_getspecific(key));
    return 0;
}

int share_lib_func(void)
{
    // pthread_t self = pthread_self();
    // print_info();
    variable += 1;
    printf("printf %p\n", printf);
    printf("This is share lib func %p %d\n", &variable, variable);

    printf("hahhah %p %d\n", &hahhah, hahhah);
   
    

    char *data = (char*)malloc(100);
    strcpy(data, "hello 123");
    // 创建键
    pthread_key_create(&key, NULL);

    // 在此线程中为该键设置一个值
    assert(pthread_setspecific(key, (void *)data) == 0);

    printf("%s\n", (char*)pthread_getspecific(key));

    return variable;
}
}