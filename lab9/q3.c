#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int start;
    int end;
} PrimeData;

bool is_prime(int num)
{
    if (num < 2)
        return false;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

void *generate_primes(void *param)
{
    PrimeData *data = (PrimeData *)param;
    for (int i = data->start; i <= data->end; i++)
    {
        if (is_prime(i))
        {
            printf("%d ", i);
        }
    }
    return NULL;
}

int main()
{
    pthread_t thread;
    PrimeData data;

    printf("Enter starting number: ");
    scanf("%d", &data.start);
    printf("Enter ending number: ");
    scanf("%d", &data.end);

    pthread_create(&thread, NULL, generate_primes, &data);
    pthread_join(thread, NULL);

    printf("\n");

    return 0;
}
