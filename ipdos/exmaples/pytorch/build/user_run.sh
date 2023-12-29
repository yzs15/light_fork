rm core*

make
export LD_LIBRARY_PATH=/home/yuzishu/Desktop/share_exp/ShareSoftState/glibc-2.35/build:/usr/local/lib/:/usr/local/lib/x86_64-linux-gnu/:/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/:/usr/lib/x86_64-linux-gnu/libfakeroot/:$LD_LIBRARY_PATH
export GLIBC_COPY_FROM_SHARE=1
time LD_PRELOAD=libsharemallocoverwrite.so  ./user