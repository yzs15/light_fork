rm core*
pkill -1 -f "./main"
# clear
gcc -o main main.c  info_util.c -lczmq -lzmq -lpthread
./main