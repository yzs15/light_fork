#include <stdlib.h>

int (*list_compare_f)(void* data1, void* data2);

struct list_t{
    void* data;
    struct list_t* next;
};

void list_init(struct list_t* head){
    head->next = NULL;
    head->data = NULL;
};

// void find()

// void add()