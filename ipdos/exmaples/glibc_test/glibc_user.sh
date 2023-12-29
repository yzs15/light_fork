clear
rm core*
export LD_LIBRARY_PATH=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build:/usr/local/lib/:/usr/local/lib/x86_64-linux-gnu/:/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/libfakeroot/:$LD_LIBRARY_PATH
ulimit -c 0
# ulimit -c unlimited
gcc -g -rdynamic  -B/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,-rpath=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,--dynamic-linker=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build/elf/ld-linux-x86-64.so.2  glibc_user.c -lipdos -lczmq -lzmq -o glibc_user

export GLIBC_COPY_FROM_SHARE=1
# export LD_DEBUG=all
# LD_PRELOAD=libipdos.so  ./glibc_user
LD_PRELOAD=libipdos.so  ./yuzishu_test user 