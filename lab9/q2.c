#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int end;
    long long sum;
} SumData;

void *calculate_sum(void *param)
{
    SumData *data = (SumData *)param;
    data->sum = 0;
    for (int i = data->start; i <= data->end; i++)
    {
        data->sum += i;
    }
    return NULL;
}

int main()
{
    pthread_t thread;
    SumData data;

    printf("Enter a non-negative integer: ");
    scanf("%d", &data.end);
    data.start = 0;

    pthread_create(&thread, NULL, calculate_sum, &data);
    pthread_join(thread, NULL);

    printf("The sum of integers from %d to %d is: %lld\n", data.start, data.end, data.sum);

    return 0;
}
