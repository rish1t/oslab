#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_FIB 100

typedef struct
{
    int n;
    long long *result;
} FibData;

void *fibonacci(void *arg)
{
    FibData *data = (FibData *)arg;
    int n = data->n;
    long long *result = data->result;

    long long a = 0, b = 1;
    for (int i = 0; i < n; i++)
    {
        result[i] = a;
        long long temp = a;
        a = b;
        b = temp + b;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > MAX_FIB)
    {
        return 1;
    }

    long long result[MAX_FIB];
    FibData data = {n, result};
    pthread_t fib_thread;
    pthread_create(&fib_thread, NULL, fibonacci, &data);
    pthread_join(fib_thread, NULL);

    for (int i = 0; i < n; i++)
    {
        printf("%lld ", result[i]);
    }
    printf("\n");

    return 0;
}
