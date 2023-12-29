cmake ../
make clean
make
rm core*
sudo cp libipdos.so /home/yuzishu/test
# ldd libipdos.so
# ./ipdos_manager_run