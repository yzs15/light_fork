// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>

// int main() {
//     int fd;
//     struct stat st;
//     void *map;

//     fd = open("./haha.txt", O_RDONLY);  // 替换为你的文件路径
//     if (fd == -1) {
//         perror("Error opening file for reading");
//         return 1;
//     }

//     fstat(fd, &st);
//     map = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
//     if (map == MAP_FAILED) {
//         close(fd);
//         perror("Error mmapping the file");
//         return 1;
//     }

//     // 对 map 指向的内存进行操作

//     munmap(map, st.st_size);
//     close(fd);

//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    struct timespec start, end;
    const int N = 10000;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &start);  // 开始计时

    for (i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // fork 出错，打印错误信息并退出
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            // 子进程立即退出
            _exit(0);
        } else {
            // 父进程等待子进程退出
            waitpid(pid, NULL, 0);
        }
    }
    

    clock_gettime(CLOCK_MONOTONIC, &end);  // 结束计时

    // 计算总时间（单位为纳秒）
    long total = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    
    // 输出平均 fork 开销
    printf("Average fork overhead: %ld ns\n", total / N);

    clock_gettime(CLOCK_MONOTONIC, &start); 
    long x = 0;
    for (int i= 0; i<150000; i++){
        // x += i;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);  // 结束计时
    total = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    
    // 输出平均 fork 开销
    printf("Average fork overhead: %ld ns\n", total);
    return 0;
}
