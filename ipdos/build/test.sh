cmake ../
make clean
make
rm core*
sudo cp libipdos.so /usr/local/lib/
# ldd libipdos.so
./ipdos_util_test