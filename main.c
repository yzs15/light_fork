#include <stdio.h>
#include <stdlib.h>
extern int ipdos_set_mem_file_name(const char* filename);
extern int ipdos_remove_parent_mem_file();
int main(){
    char* data = malloc(1024*1024*8);
    ipdos_set_mem_file_name("c.mem");
    int pid = fork();
    if (pid == 0) {
        // Child process
        printf("Hello from child process\n");
        while (1) {
            sleep(1);
        }
    } else {
        // Parent process
        printf("Hello from parent process\n");  
        printf("child pid: %d\n", pid);

    }
}