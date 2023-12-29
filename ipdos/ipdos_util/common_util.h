#include <stdbool.h>
bool ipdos_util_set_main_thread();
// bool ipdos_is_main_thread();

enum ipdos_identity{
    IPDOS_NONE,
    IPDOS_USER,
    IPDOS_PROVIDER
};

bool ipdos_util_set_identity(enum ipdos_identity identity);

bool ipdos_util_is_user();

bool ipdos_util_is_provider();
int ipdos_util_is_shared_ptr(void* ptr);