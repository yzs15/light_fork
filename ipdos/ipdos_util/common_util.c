#include "ipdos_util/common_util.h"
#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include "ipdos_util/info_util.h"

pthread_t ipdos_util_main_thread_id = -1;

bool ipdos_util_set_main_thread(){
    assert(ipdos_util_main_thread_id == -1);
    ipdos_util_main_thread_id = pthread_self();
    return true;
}

// bool ipdos_is_main_thread(){
//     assert(ipdos_util_main_thread_id != -1);
//     return pthread_self() == ipdos_util_main_thread_id;
// }


enum ipdos_identity ipdos_util_identity = IPDOS_NONE;
bool ipdos_util_set_identity(enum ipdos_identity identity){
    assert(identity == IPDOS_USER || identity == IPDOS_PROVIDER);
    ipdos_util_identity = identity;
    return true;
}

bool ipdos_util_is_user(){
    return ipdos_util_identity == IPDOS_USER;
}

bool ipdos_util_is_provider(){
    return ipdos_util_identity == IPDOS_PROVIDER;
}

//判断一个指针是不是在共享内存区域
int ipdos_util_is_shared_ptr(void* ptr){
    if((uintptr_t)ptr>=SHARE_MEM_FIX_ADDR_BASE && (uintptr_t)ptr<SHARE_MEM_FIX_ADDR_TOP){
        return 1;
    }
    return 0;
}

