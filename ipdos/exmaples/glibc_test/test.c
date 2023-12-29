#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_key_t key;

void* thread_func1(void* arg) {
    int* p = (int*)malloc(sizeof(int));
    *p = 1;
    printf("Thread 1: %d %p %p\n",key, &key, pthread_getspecific(key));
    pthread_setspecific(key, p);
    printf("Thread 1: %d %p %d\n",key, &key, *(int*)pthread_getspecific(key));
    return NULL;
}

void* thread_func2(void* arg) {
    int* p = (int*)malloc(sizeof(int));
    *p = 2;
    printf("Thread 2: %d %p %p\n", key, &key, pthread_getspecific(key));
    pthread_setspecific(key, p);
    printf("Thread 2: %d %p %d\n", key, &key, *(int*)pthread_getspecific(key));
    return NULL;
}

void key_destructor(void* value) {
    free(value);
}

int main() {
    pthread_t thread1, thread2;
    pthread_key_create(&key, key_destructor);

    int* p = (int*)malloc(sizeof(int));
    *p = 3;
    pthread_setspecific(key, p);

    pthread_create(&thread1, NULL, thread_func1, NULL);
    pthread_create(&thread2, NULL, thread_func2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Thread 0: %d %p %d\n", key, &key, *(int*)pthread_getspecific(key));
    pthread_key_delete(key);

    return 0;
}
