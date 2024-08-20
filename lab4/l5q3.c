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
        printf("\nCurrently in Child : \n");
        printf("pid:%ld\tppid:%ld\n", (long)getpid(), (long)getppid());
        exit(0);
    }
    else
    {
        printf("\nCurrently in Parent : \n");
        printf("pid:%ld\t%ld\n", (long)getpid(), (long)getppid());
        exit(0);
    }
}