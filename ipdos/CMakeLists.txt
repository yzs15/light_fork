cmake_minimum_required(VERSION 3.16)

project(IpdosProject)

set(CMAKE_C_STANDARD 99)

add_definitions("-Wall -g")
include_directories(.)


# add_executable(ipdos ipdos/main.c ipdos_util/info_util.c)
set(SOURCE_FILES ipdos_util/info_util.c ipdos/ipdos.c ipdos/ipdos_client.c ipdos/sharemalloc_dlmalloc.c ipdos/dlmalloc.c
    ipdos_util/common_util.c ipdos/pthread_proxy.c)
add_library(ipdos SHARED ${SOURCE_FILES})
target_link_libraries(ipdos  "-lpthread")

add_executable(ipdos_manager_run ipdos_manager/main.c ipdos_util/info_util.c)
target_link_libraries(ipdos_manager_run "-lpthread")

add_executable(ipdos_util_test  ipdos_util/test_info_util.c ipdos_util/info_util.c)
