#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, write_sem;
int read_count = 0;

void *reader(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++)
    {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
        {
            sem_wait(&write_sem);
        }
        sem_post(&mutex);

        printf("Reader %d is reading\n", id);
        sleep(1);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
        {
            sem_post(&write_sem);
        }
        sem_post(&mutex);
        sleep(1);
    }

    return NULL;
}

void *writer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++)
    {
        sem_wait(&write_sem);

        printf("Writer %d is writing\n", id);
        sleep(2);

        sem_post(&write_sem);

        sleep(2);
    }

    return NULL;
}

int main()
{
    pthread_t readers[3], writers[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};

    sem_init(&mutex, 0, 1);
    sem_init(&write_sem, 0, 1);

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&write_sem);

    return 0;
}
