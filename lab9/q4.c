#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    int size;
    long long sum;
    int is_even;
} SumData;

void *calculate_sum(void *param)
{
    SumData *data = (SumData *)param;
    data->sum = 0;
    for (int i = 0; i < data->size; i++)
    {
        if (data->is_even && data->array[i] % 2 == 0)
        {
            data->sum += data->array[i];
        }
        else if (!data->is_even && data->array[i] % 2 != 0)
        {
            data->sum += data->array[i];
        }
    }
    return NULL;
}

int main()
{
    int size;

    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int *array = (int *)malloc(size * sizeof(int));
    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &array[i]);
    }

    pthread_t even_thread, odd_thread;
    SumData even_data = {array, size, 0, 1};
    SumData odd_data = {array, size, 0, 0};

    pthread_create(&even_thread, NULL, calculate_sum, &even_data);
    pthread_create(&odd_thread, NULL, calculate_sum, &odd_data);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    printf("Sum of even numbers: %lld\n", even_data.sum);
    printf("Sum of odd numbers: %lld\n", odd_data.sum);

    free(array);
    return 0;
}
