cmake ../
make clean
make
rm core*
cp libipdos.so ../../
# ldd libipdos.so
# ./ipdos_manager_run