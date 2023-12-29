cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(provider)
add_compile_options(-g)
set(CMAKE_CXX_STANDARD 14)
find_package(Torch REQUIRED)

add_executable(provider provider.cpp)
target_link_libraries(provider  "-lipods")

add_executable(user user.cpp)
target_link_libraries(user  "-lipods")

set(SOURCE_FILES service.cpp)
add_library(service SHARED ${SOURCE_FILES})
set(GLIBC_PATH "/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build")
set(GLIBC_LD "${GLIBC_PATH}/elf/ld-linux-x86-64.so.2")
target_link_libraries(service "${TORCH_LIBRARIES}")
# "${TORCH_LIBRARIES}"
# set_property(TARGET provider PROPERTY CXX_STANDARD 14)
set_property(TARGET user PROPERTY LINK_FLAGS "-Wl,-rpath=${GLIBC_PATH},--dynamic-linker=${GLIBC_LD}")
set_property(TARGET provider PROPERTY LINK_FLAGS "-Wl,-rpath=${GLIBC_PATH},--dynamic-linker=${GLIBC_LD}")