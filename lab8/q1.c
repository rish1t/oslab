#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buf[11], f = 0, r = 0;
sem_t mutex, full, empty, items_in_buffer;

void *produce(void *arg)
{
    int i;
    for (i = 0; i < 11; i++)
    {
        sem_wait(&items_in_buffer);
        sem_wait(&empty);
        sem_wait(&mutex);
        printf("produced item is %d\n", i);
        buf[(++r) % 11] = i;
        sleep(1);
        sem_post(&mutex);
        sem_post(&full);
        // sem_post(&items_in_buffer);
        int value;
        sem_getvalue(&items_in_buffer, &value);
        printf("diff is %d\n", value);
        if (value == 0)
        {
            printf("Procuder is 10 ahead, stopping...\n");
        }
    }
    return NULL;
}

void *consume(void *arg)
{
    int item, i;
    for (i = 0; i < 11; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        item = buf[(++f) % 11];
        printf("consumed item is %d\n", item);
        sleep(1);
        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&items_in_buffer);
        int value;
        sem_getvalue(&items_in_buffer, &value);
        if (value > 3)
        {
            return;
        }
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 11);
    sem_init(&items_in_buffer, 0, 10);

    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&items_in_buffer);

    return 0;
}
