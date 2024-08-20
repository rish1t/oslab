#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	int status;
	pid_t pid;
	pid = fork();
	if(pid == -1)
		perror("Error child not created.");
	else if (pid == 0) 
	{
		printf("\nI'm the child!");
		exit(0);
	}
	else 
	{	
		wait(&status);
		printf("\nI'm the parent!");
		printf("\nChild returned: %d\n", status);
	}
}