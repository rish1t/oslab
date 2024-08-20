#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int status;

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
        exit(0);
    }
    else
    {
        wait(&status);

        printf("\nParent: Child exited with status code %d\n", status);
    }

    return 0;
}
