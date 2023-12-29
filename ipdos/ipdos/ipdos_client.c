#include "ipdos/ipdos_client.h"
#include <ipdos_util/debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// struct ipdos_client_t* ipdos_client_create(char* addr){
//     struct ipdos_client_t* client = malloc(sizeof(struct ipdos_client_t));
//     // client->clinet_sock = zsock_new_req (addr);
//     client->zmq_ctx = zmq_ctx_new ();
//     client->clinet_sock = zmq_socket(client->zmq_ctx, ZMQ_REQ);
//     zmq_connect(client->clinet_sock , addr);
//     return client;
// }

// void ipdos_client_destroy(struct ipdos_client_t* client){
//     zmq_close(client->clinet_sock);
//     zmq_ctx_term(client->zmq_ctx);
//     // zsock_destroy(&(client->clinet_sock));
//     free(client);
// }

//如果返回是NULL，说明分配失败
//需要free返回的指针
struct service_info_t* ipdos_client_alloc_addr(char* unix_path, char* name, char* version, char* file_path){
    struct sockaddr_un addr;
    int sock;
    char buffer[4096];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, unix_path, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(1);
    }
    sprintf(buffer, "%s %s %s", name, version, file_path);
    write(sock, buffer, strlen(buffer)+1);

    unsigned long start_addr;
    unsigned long end_addr;
    read(sock, buffer, sizeof(buffer));
    //  printf("Received: %s\n", buffer);
    sscanf(buffer, "%ld %ld", &start_addr, &end_addr);
    // printf("%lx, %lx\n", start_addr, end_addr);
    
    struct service_info_t service_info = {
        .service_name = name,
        .version = version,
        .shared_state_file_path = file_path,
        .start_virtual_addr = start_addr,
        .end_virtual_addr = end_addr
    };  
    struct service_info_t* info_ret = service_info_copy(&service_info);
    close(sock);
    return info_ret;

}

//如果返回是NULL，说明没有这个特定版本的服务
//需要free返回的指针
// struct service_info_t* ipdos_client_query(struct ipdos_client_t* client, char* name, char* version){
    //发送查询请求
    // zmsg_t* msg = zmsg_new();
    // zmsg_addmem(msg, "query", strlen("query")+1);
    // zmsg_addmem(msg, name, strlen(name)+1);
    // zmsg_addmem(msg, version, strlen(version)+1);
    // zmsg_send(&msg, client->clinet_sock);

    // //接收查询结果
    // zmsg_t* msg_rep = zmsg_recv(client->clinet_sock);
    // zframe_t* frame_status = zmsg_next (msg_rep);
    // zmsg_remove (msg_rep, frame_status);
    // int32_t status = *((int32_t*) zframe_data(frame_status));
    // zframe_destroy(&frame_status);
    // if (status != STATUS_OK){
    //     zmsg_destroy(&msg_rep);
    //     return NULL;
    // }
    // int size = zmsg_size (msg_rep);
    // if (size != 3){
    //     LG("size error");
    //     zmsg_destroy(&msg_rep);
    //     return NULL;
    // }
    // zframe_t* frame_file_path = zmsg_next (msg_rep);
    // zmsg_remove (msg_rep, frame_file_path);
    // zframe_t* frame_start_addr = zmsg_next (msg_rep);
    // zmsg_remove (msg_rep, frame_start_addr);
    // zframe_t* frame_end_addr = zmsg_next (msg_rep);
    // zmsg_remove (msg_rep, frame_end_addr);
    // unsigned long start_addr = *((unsigned long *) zframe_data(frame_start_addr));
    // unsigned long end_addr = *((unsigned long *) zframe_data(frame_end_addr));
    // struct service_info_t info = {
    //     .service_name=name,
    //     .version=version,
    //     .shared_state_file_path= (char*)zframe_data(frame_file_path),
    //     .start_virtual_addr=start_addr,
    //     .end_virtual_addr=end_addr,
    //     .share_mmap_list_head = NULL,
    //     .dl_open_list_head = NULL,
    //     .shared_mspace = NULL,
    //     .ld_so_hared_map_ptr = NULL,
    //     .shared_so_loaded = 0,
    //     .shared_so_nsid = -1
    // };

    // struct service_info_t* info_ret = service_info_copy(&info);
    // zframe_destroy(&frame_file_path);
    // zframe_destroy(&frame_start_addr);
    // zframe_destroy(&frame_end_addr);
    // zmsg_destroy(&msg_rep);
    // return info_ret;
//     fprintf(stdout, "[WARNNING](File: %s:%d, Func: %s()): 没有实现\n", __FILE__,__LINE__, __FUNCTION__);
//     return NULL;
// }


struct service_info_t* ipdos_query_from_file(char* file_path, char* name, char* version){
    FILE* f=NULL;
    while(true){
        f = fopen(file_path, "r");
        if (f != NULL){
            break;
        }
    }
    // printf("ipdos_query_from_file %p %s\n", f, file_path);
    char stored_name[100], stored_version[100];
    char stored_path[1024];
    long start, end;
    while(fscanf(f, "%s %s %s %ld %ld", stored_name, stored_version, stored_path, &start, &end) == 5){
        if (strcmp(name, stored_name) == 0 && strcmp(version, stored_version) == 0){

            struct service_info_t info = {
                .service_name=name,
                .version=version,
                .shared_state_file_path= stored_path,
                .start_virtual_addr=start,
                .end_virtual_addr=end,
                .share_mmap_list_head = NULL,
                .dl_open_list_head = NULL,
                .shared_mspace = NULL,
                .ld_so_hared_map_ptr = NULL,
                .shared_so_loaded = 0,
                .shared_so_nsid = -1
            };
            // printf("%s %s %s %lx %lx\n", stored_name, stored_version, stored_path, start, end);
            struct service_info_t* info_ret = service_info_copy(&info);
            return info_ret;
        }
    }
    fclose(f);
    // printf("ipdos_query_from_file %d\n", ret);
    return NULL;
}