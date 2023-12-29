export GLIBC_USE_SHARED_MALLOC=1
export LD_LIBRARY_PATH=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build:/usr/local/lib/:/usr/local/lib/x86_64-linux-gnu/:/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/libfakeroot/:$LD_LIBRARY_PATH
gcc -g -rdynamic -B/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,-rpath=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build -Wl,--dynamic-linker=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build/elf/ld-linux-x86-64.so.2  memory_test.c -lipdos  -o memory_test
rm core*
ulimit -c unlimited

export IPDOS_MANAGER_ADDR="/tmp/example.sock"
LD_PRELOAD=libipdos.so  ./memory_test
