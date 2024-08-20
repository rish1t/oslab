#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("\nChild process created with id : %d\nParent : %d", getpid(), getppid());
        sleep(3);
        printf("\nChild process : %d\nParent : %d", getpid(), getppid());
    }
    else
    {
        printf("Parent Complete");
        exit(0);
    }
}