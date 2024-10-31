#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;

using ull = unsigned long long;

uint64_t my_hash(ull *ptr, size_t len) {
    uint64_t hash = 0;
    for (size_t i = 0; i < len; i++) {
        hash = hash * 131 + ptr[i];
    }
    return hash;
}

int main() {
    auto fd = open("/tmp/test0", O_RDWR | O_CREAT, 0644);
    auto fd1 = open("/tmp/test1", O_RDWR | O_CREAT, 0644);

    const auto size = 1024 * 1024 * sizeof(ull);
    const auto size2 = size / 2;

    auto *ptr1 =
        (ull *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < size / sizeof(ull); i++)
        ptr1[i] = i;
    auto hash1 = my_hash((ull *)ptr1, size / sizeof(ull));

    printf("addr = 0x%lx, hash = %lu\n", (long)ptr1, hash1);

    auto *ptr2 = (ull *)mmap(ptr1, size2, PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_FIXED, fd1, 0);
    auto hash2 = my_hash((ull *)ptr2, size / sizeof(ull));

    printf("addr = 0x%lx, hash = %lu\n", (long)ptr2, hash2);

    ptr2[0] = 0x5a5a5a5a5a5a5a5aull;
    *(ull *)((char *)ptr2 + size - sizeof(ull)) = 0x5a5a5a5a5a5a5a5aull;

    close(fd);
    close(fd1);

    return 0;
}
