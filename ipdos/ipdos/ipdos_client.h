// #include <czmq.h>
#include "ipdos_util/info_util.h"
#define STATUS_OK 200
#define STATUS_FAILED 100


// struct ipdos_client_t{
//     zsock_t *clinet_sock;
//     void *zmq_ctx;
// };

// struct ipdos_client_t* ipdos_client_create(char* addr);
// void ipdos_client_destroy(struct ipdos_client_t* client);
struct service_info_t* ipdos_client_alloc_addr(char* unix_path, char* name, char* version, char* file_path);
// struct service_info_t* ipdos_client_query(struct ipdos_client_t* client, char* name, char* version);
struct service_info_t* ipdos_query_from_file(char* file_path, char* name, char* version);