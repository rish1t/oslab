#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem1, sem2;

void *thread1(void *arg)
{
    printf("Thread 1: Trying to acquire sem1...\n");
    sem_wait(&sem1);
    printf("Thread 1: Acquired sem1, now trying to acquire sem2...\n");
    sleep(1);
    sem_wait(&sem2);
    printf("Thread 1: Acquired sem2\n");
    sem_post(&sem2);
    sem_post(&sem1);
    return NULL;
}

void *thread2(void *arg)
{
    printf("Thread 2: Trying to acquire sem2...\n");
    sem_wait(&sem2);
    printf("Thread 2: Acquired sem2, now trying to acquire sem1...\n");
    sleep(1);
    sem_wait(&sem1);
    printf("Thread 2: Acquired sem1\n");

    sem_post(&sem1);
    sem_post(&sem2);
    return NULL;
}

int main()
{
    pthread_t t1, t2;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
