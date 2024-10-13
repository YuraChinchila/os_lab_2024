#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        printf("Child pid [%d]\n", getpid());
        exit(0);
    }

    sleep(10);
    printf("Parent finished\n");
    return 0;
}
