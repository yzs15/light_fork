
make clean
clear
cmake -DCMAKE_PREFIX_PATH=/opt/libtorch ..
make

rm core*
rm /dev/shm/my_shared_memory 
rm /home/yuzishu/glibc_share_memory
rm /home/yuzishu/my_shared_memory 

export LD_LIBRARY_PATH=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build:/usr/local/lib/:/usr/local/lib/x86_64-linux-gnu/:/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/libfakeroot/:$LD_LIBRARY_PATH
export GLIBC_USE_SHARED_MALLOC=1
# ldd provider
# ldd libservice.so
# time LD_PRELOAD=libipods.so  ./provider
# ./provider