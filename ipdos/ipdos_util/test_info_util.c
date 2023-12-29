#include <stdio.h>
#include <stdlib.h>
#include "ipdos_util/info_util.h"

int main(){
    struct service_map_t map;
    service_info_map_init(&map, 1);
    struct service_info_t info1 = {
        service_name:"libservice",
        version:"1.1.1",
        shared_state_file_path:"/home/yuzishu/shared/libservice_1.1.1_share",
        start_virtual_addr:0x7f4500000000,
        end_virtual_addr:  0x7f5000000000,
    };
    service_info_put_no_force(&map, &info1);
    struct service_info_t info2 =  {
        service_name:"libservice",
        version:"1.1.2",
        shared_state_file_path:"/home/yuzishu/shared/libservice_1.1.2_share",
        start_virtual_addr:0x7f5000000000,
        end_virtual_addr:  0x7f5500000000,
    };
    service_info_put_no_force(&map, &info2);
   
    struct service_info_t info3 =  {
        service_name:"libservice",
        version:"1.1.2",
        shared_state_file_path:"/",
        start_virtual_addr:0x7f5000000000,
        end_virtual_addr:  0x7f5500000000,
    };
    service_info_put_no_force(&map, &info3);

    // service_info_print(service_info_get(&map, "libservice", "1.1.2"));
    // service_info_print(service_info_get(&map, "libservice", "1.1.1"));
    service_info_print(service_info_get_by_addr(&map, (void*)(uintptr_t)0x7f5000000000));
}