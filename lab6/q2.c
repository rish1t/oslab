#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define num 6

int main()
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    int pfd[2];
    pid_t cpid;

    if (pipe(pfd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
        close(pfd[1]);

        int received_arr[num];
        read(pfd[0], received_arr, sizeof(received_arr));

        int sum = 0;
        for (int i = 0; i < num; i++)
        {
            sum += received_arr[i];
        }

        close(pfd[0]);
        printf("The sum of the array is: %d\n", sum);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pfd[0]);
        write(pfd[1], arr, sizeof(arr));
        close(pfd[1]);
        wait(NULL);

        exit(EXIT_SUCCESS);
    }
}
