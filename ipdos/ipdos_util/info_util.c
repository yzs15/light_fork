#include "ipdos_util/hashmap.h"
#include "ipdos_util/info_util.h"
#include <stdio.h>
#include <assert.h>
int service_info_map_init(struct service_map_t *service_map, const unsigned initial_size){
    if (0 != hashmap_create(initial_size, &(service_map->hashmap))) {
      return 0;
    }
    return 1;
}


//返回serviuce的key，需要在函数外面free返回的key指针
//set key_len, which is the length of key
char* service_info_key_gen(char* service_name, char* version, size_t* key_len){
    size_t name_len = strlen(service_name);
    size_t version_len =  strlen(version);
    *key_len = name_len + version_len;
    char *key = malloc(name_len + version_len + 1);
    memcpy(key, service_name, name_len);
    memcpy(key+name_len, version, version_len);
    key[name_len + version_len] = '\0';
    return key;
}

int service_info_put_force(struct service_map_t *service_map, struct service_info_t* service_info){
    size_t key_len;
    service_info_del(service_map, service_info->service_name, service_info->version);
    char *key = service_info_key_gen(service_info->service_name, service_info->version, &key_len);
    struct service_info_t* new = service_info_copy(service_info);
    if (0 != hashmap_put(&(service_map->hashmap), key, key_len, new)) {
        return 0;
    }
    return 1;
}


int service_info_put_no_force(struct service_map_t *service_map, struct service_info_t* service_info){
    size_t key_len;
    char *key = service_info_key_gen(service_info->service_name, service_info->version, &key_len);

    if (service_info_get(service_map, service_info->service_name, service_info->version) != NULL){
        return 0;
    }
    struct service_info_t* new = service_info_copy(service_info);
    if (0 != hashmap_put(&(service_map->hashmap), key, key_len, new)) {
        return 0;
    }
    return 1;
}


//NULL for fail
struct service_info_t* service_info_get(struct service_map_t *service_map, char* service_name, char* version){
    size_t key_len;
    char *key = service_info_key_gen(service_name, version, &key_len);
    void* element = hashmap_get(&(service_map->hashmap), key,key_len);
    return (struct service_info_t*) element;
}

struct service_info_get_by_addr_t
{
    void* addr;
    struct service_info_t* info;
};


static int service_info_get_by_addr_iterate(void* const context, void* const value) {

    struct service_info_t* info = ( struct service_info_t*) value;
    struct service_info_get_by_addr_t**  return_info_ptr = (struct service_info_get_by_addr_t**) context;
    struct service_info_get_by_addr_t*  return_info = *return_info_ptr;
    // printf("----------------------------------\n");
    // service_info_print(info);
    // printf("addr: %lx %d %d\n", (uintptr_t)return_info->addr,(uintptr_t)return_info->addr >= info->start_virtual_addr,
        // (uintptr_t)return_info->addr < info->end_virtual_addr);

    if ((uintptr_t)return_info->addr >= info->start_virtual_addr && (uintptr_t)return_info->addr < info->end_virtual_addr) {
        // Store into our user-provided context the value.
        return_info->info = info;
        // printf("find\n");
        // Return 0 to tell the iteration to stop here.
        return 0;
    }
    // Otherwise tell the iteration to keep going.
    return 1;
}


struct service_info_t* service_info_get_by_addr(struct service_map_t *service_map, void* addr){
    LG("addr %p", addr);
    struct service_info_get_by_addr_t*  info = malloc(sizeof(struct service_info_get_by_addr_t));
    info->addr = addr;
    if (0 != hashmap_iterate(&(service_map->hashmap), service_info_get_by_addr_iterate, &info)) {
        return info->info;
    } else {
        return NULL;
    }
}

//1 success; 0 failed
int service_info_del(struct service_map_t *service_map, char* service_name, char* version){
    size_t key_len;
    char *key = service_info_key_gen(service_name, version, &key_len);
    struct service_info_t* info = service_info_get(service_map, service_name,  version);
    if (info == NULL){
        return 1;
    }
    service_info_free(info);
    return hashmap_remove(&(service_map->hashmap), key,key_len) == 0;
}

void service_info_print( struct service_info_t* service_info){
    if (service_info == NULL){
        return;
    }
    printf("service_name: %s\n", service_info->service_name);
    printf("version %s\n", service_info->version);
    printf("shared_state_file_path: %s\n", service_info->shared_state_file_path);
    printf("start_virtual_addr: %lx\n", service_info->start_virtual_addr);
    printf("end_virtual_addr: %lx\n", service_info->end_virtual_addr);
}

struct service_info_t* service_info_copy(struct service_info_t* service_info){
    struct service_info_t* new = malloc(sizeof( struct service_info_t));
    *new = *service_info;

    size_t name_len = strlen( service_info->service_name)+1;
    new->service_name = malloc(name_len);
    memcpy(new->service_name,  service_info->service_name, name_len);

    size_t verison_len = strlen( service_info->version)+1;
    new->version = malloc(verison_len);
    memcpy(new->version,  service_info->version, verison_len);


    size_t path_len = strlen( service_info->shared_state_file_path)+1;
    new->shared_state_file_path = malloc(path_len);
    memcpy(new->shared_state_file_path,  service_info->shared_state_file_path, path_len);
    
    new->start_virtual_addr = service_info->start_virtual_addr;
    new->end_virtual_addr = service_info->end_virtual_addr;
    return new;
}

void service_info_free(struct service_info_t* service_info){
    free(service_info->service_name);
    free(service_info->version);
    free(service_info->shared_state_file_path);
    free(service_info);
}


struct service_map_t *service_info_user_service_map = NULL;
bool service_info_set_user_service_map(struct service_map_t *service_map){
    assert(service_info_user_service_map == NULL);
    service_info_user_service_map = service_map;
    return true;
}
struct service_map_t * service_info_get_user_service_map(){
    assert(service_info_user_service_map != NULL);
    return service_info_user_service_map;
}
