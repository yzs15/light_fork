#ifndef SHEREDOM_LIST_H_INCLUDED
#define SHEREDOM_LIST_H_INCLUDED

#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

struct list_head_t {
    pthread_mutex_t lock;
    struct list_item_t* next;
};

struct list_item_t{
    void* data;
    struct list_item_t* next;
};

__attribute__((weak)) void list_init(struct list_head_t* head){
    head->next = NULL;
    assert(pthread_mutex_init(&(head->lock), NULL) ==0);
    
};

__attribute__((weak)) void list_add(struct list_head_t* head, void* data){
    struct list_item_t* l =  ( struct list_item_t*) malloc(sizeof(struct list_item_t));
    l->data = data;
    pthread_mutex_lock(&(head->lock));
    l->next = head->next;
    head->next = l;
    pthread_mutex_unlock(&(head->lock));
}


typedef void (*list_iter_func_t)(void *);
__attribute__((weak)) void list_map(struct list_head_t* head, list_iter_func_t func){
    struct list_item_t* l;
    if (head->next != NULL){
        l = head->next;
    } 
    while (l != NULL){
        func(l->data);
        l = l->next;
    }
}
#endif