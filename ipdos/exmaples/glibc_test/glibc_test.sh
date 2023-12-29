# ../configure --prefix=/opt/glibc
clear
rm yuzishu_test
rm /home/yuzishu/glibc_share_memory
rm /home/yuzishu/my_shared_memory 
rm /dev/shm/my_shared_memory
rm core*
# bash dlmalloc_runtest_overwrite.sh
rm /home/yuzishu/share_folder/libsharetest1.0.0
rm /home/yuzishu/share_folder/libsharetest2.0.0

g++ -o libsharetest.so -shared -fPIC -g test_share_lib.cpp -lrt -lpthread
g++ -o libsharetestfather.so -shared -fPIC -g test_share_lib_father.cpp -lrt -lpthread -ldl
# rm /dev/shm/my_shared_memory 
# rm /my_shared_memory
# export LD_LIBRARY_PATH=/opt/glibc/lib:$LD_LIBRARY_PATH 
# export LIBRARY_PATH=/opt/glibc/lib:$LIBRARY_PATH
# export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build:/usr/local/lib/:/usr/local/lib/x86_64-linux-gnu/:/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/libfakeroot/:$LD_LIBRARY_PATH
ulimit -c unlimited
# sudo ldconfig
gcc -g -rdynamic -B/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,-rpath=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,--dynamic-linker=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build/elf/ld-linux-x86-64.so.2  glibc_test.c -lipdos -lczmq -lzmq -o yuzishu_test
export GLIBC_USE_SHARED_MALLOC=1
# ldd yuzishu_test
pkill -1 -f yuzishu_test

# sleep 1
# export LD_DEBUG=all
LD_PRELOAD=libipdos.so  ./yuzishu_test provider 2.0.0 
# sleep 1
# LD_PRELOAD=libipdos.so  ./yuzishu_test 1.0.0 

wait