// #include <czmq.h>
#include "ipdos_util/info_util.h"
#include <sys/mman.h>
#include "ipdos_util/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_PATH "/tmp/example.sock"


#define STATUS_OK 200
#define STATUS_FAILED 100

#define SHARE_MEM_FIX_ADDR_BASE 0x600000000000
#define SHARE_MEM_FIX_ADDR_TOP  0x7f0000000000



int handle_virtual_addr_alloc(int client_sock, char* buffer, struct service_map_t *map);
// int handle_virtual_addr_query(zsock_t *rep_handler, zmsg_t* msg,struct service_map_t *map);
// int handle_request(zsock_t *rep_handler, struct service_map_t *map);

unsigned long Virtual_Addr_Start = SHARE_MEM_FIX_ADDR_BASE;
unsigned long Virtual_Addr_Alloc_Step= 0x800000000;//32GB 
// 0x800000000000
// zsock_t* test_clinet_sock;
FILE * ipdos_manager_log_fd = NULL;
int main (void)
{
    // zsock_t *rep_handler = NULL;
    struct service_map_t map;
    // rep_handler= zsock_new_rep ("tcp://0.0.0.0:5555");
    
    ipdos_manager_log_fd = fopen("/dev/shm/lass_ict/ipdos_manager_log", "w");
    // printf("%d\n", ipdos_manager_log_fd);
    service_info_map_init(&map, 10);

    struct sockaddr_un addr;
    int server_sock, client_sock;
    char buffer[1024];

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, UNIX_PATH, sizeof(addr.sun_path) - 1);

    unlink(UNIX_PATH);

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server_sock, 5) == -1) {
        perror("listen");
        exit(1);
    }

    while (1) {
        client_sock = accept(server_sock, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            exit(1);
        }

        // 读取两个字符串
        read(client_sock, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);
        
        handle_virtual_addr_alloc(client_sock, buffer, &map);

        close(client_sock);
    }

    close(server_sock);

    return 0;

    // while(1){
    //      handle_request(rep_handler, &map);
    // }
    // zsock_destroy (&rep_handler);
    // return 0;
}


//if already alloc virtual addr, return the addr; else return new alloced addr
//request name version path
//reply start_addr end_addr
int handle_virtual_addr_alloc(int client_sock, char* buffer, struct service_map_t *map){
    char name[128], version[128], path[1024];

    sscanf(buffer, "%s %s %s", name, version, path);
    unsigned long start_addr =0;
    unsigned long end_addr =0;

    struct service_info_t *info = service_info_get(map, name, version);
    if (info != NULL){
        start_addr = info->start_virtual_addr;
        end_addr = info->end_virtual_addr;

    }else{
        start_addr = Virtual_Addr_Start;
        Virtual_Addr_Start += Virtual_Addr_Alloc_Step;
        end_addr = Virtual_Addr_Start-1024;
        struct service_info_t tmp = {
            service_name:name,
            version:version,
            shared_state_file_path:path,
            start_virtual_addr:start_addr,
            end_virtual_addr:end_addr,
        };

        service_info_put_force(map, &tmp);
        fprintf(ipdos_manager_log_fd,  "%s %s %s %ld %ld\n", name, version, path, start_addr, end_addr);
        fflush(ipdos_manager_log_fd);
    }
  
    sprintf(buffer, "%ld %ld",start_addr, end_addr);
    write(client_sock, buffer, strlen(buffer)+1);
    // close(client_sock);
    return 0;
    
}



// //name version path
// int handle_request(zsock_t *rep_handler, struct service_map_t *map){
//     // printf("handle_request\n");
//     zmsg_t* msg = zmsg_recv(rep_handler);
//     int size = zmsg_size (msg);
//     // printf("error size= %d\n", size);
//     if (size < 2){
//         return 1;
//     }

//     int ret=1;
//     zframe_t* frame = zmsg_next (msg);
//     char* req_type = (char*)zframe_data(frame);
//     if (strcmp("alloc", req_type)==0){
//         zmsg_remove (msg, frame);
//         zframe_destroy(&frame);
//         ret = handle_virtual_addr_alloc(rep_handler, msg, map);
//         zmsg_destroy(&msg);
//     }

//     if (strcmp("query", req_type)==0){
//         zmsg_remove (msg, frame);
//         zframe_destroy(&frame);
//         ret= handle_virtual_addr_query(rep_handler, msg, map);
//         zmsg_destroy(&msg);
//     }
//     return ret;

    
// }


// int handle_virtual_addr_query(zsock_t *rep_handler, zmsg_t* msg,struct service_map_t *map){
//     int size = zmsg_size (msg);
//     if (size != 2){
//         //TODO reply error
//         return 1;
//     }

//     zframe_t* frame_name = zmsg_next (msg);
//     char* name = (char*)zframe_data(frame_name);
//     zmsg_remove (msg, frame_name);

//     zframe_t* frame_version = zmsg_next (msg);
//     char* version = (char*) zframe_data(frame_version);
//     zmsg_remove (msg, frame_version);

//     printf("%s, %s\n", name, version);

//     struct service_info_t* info = service_info_get(map, name, version);
//     if (info == NULL){
//         int32_t status = STATUS_FAILED;
//         zmsg_t* rep_msg = zmsg_new();
//         zmsg_addmem(rep_msg, &status, sizeof(int32_t)); 
//         zmsg_send(&rep_msg, rep_handler);
//     }

//     zmsg_t* rep_msg = zmsg_new();
//     int32_t status = STATUS_OK;
//     zmsg_addmem(rep_msg, &status, sizeof(int32_t)); 
//     zmsg_addmem(rep_msg, info->shared_state_file_path, strlen(info->shared_state_file_path)+1); 
//     zmsg_addmem(rep_msg, &(info->start_virtual_addr), sizeof(unsigned long)); 
//     zmsg_addmem(rep_msg, &(info->end_virtual_addr), sizeof(unsigned long));
//     zmsg_send(&rep_msg, rep_handler);
    
//     return 0;
// }

