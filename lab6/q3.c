#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        close(fd[1]);

        char buf[BUFFER_SIZE];
        int n;
        int vowel_count = 0;

        if ((n = read(fd[0], buf, BUFFER_SIZE - 1)) >= 0)
        {
            buf[n] = '\0';
            int spaces = 0;

            for (int i = 0; i < n; i++)
            {
                switch (buf[i])
                {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                case 'A':
                case 'E':
                case 'I':
                case 'O':
                case 'U':
                    vowel_count++;
                    break;
                case ' ':
                    spaces++;
                    break;
                default:
                    break;
                }
            }

            printf("Number of vowels: %d\n", vowel_count);
            printf("Number of consonants: %d\n", n - vowel_count - spaces);
        }
        else
        {
            perror("read");
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(fd[0]);

        char *data = "hi I love OS lab on tuesday";
        write(fd[1], data, strlen(data));

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}
