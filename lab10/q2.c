#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TIME_QUANTUM 4

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int queue_level;
    struct process *next;
};

typedef struct process Process;
typedef Process* ProcessPtr;

ProcessPtr queue1 = NULL;
ProcessPtr queue2 = NULL;
ProcessPtr queue3 = NULL;

ProcessPtr createProcess(int pid, int arrival_time, int burst_time) {
    ProcessPtr newProcess = (ProcessPtr) malloc(sizeof(Process));
    newProcess->pid = pid;
    newProcess->arrival_time = arrival_time;
    newProcess->burst_time = burst_time;
    newProcess->remaining_time = burst_time;
    newProcess->queue_level = 1;
    newProcess->next = NULL;
    return newProcess;
}

void insertProcess(ProcessPtr *queue, ProcessPtr process) {
    if (*queue == NULL) {
        *queue = process;
    } else {
        ProcessPtr temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = process;
    }
}

ProcessPtr removeProcess(ProcessPtr *queue) {
    if (*queue == NULL) {
        return NULL;
    }
    ProcessPtr process = *queue;
    *queue = (*queue)->next;
    process->next = NULL;
    return process;
}

void scheduleQueue1() {
    if (queue1 == NULL) {
        return;
    }

    ProcessPtr currentProcess = removeProcess(&queue1);
    printf("Queue 1 (Round Robin): Running process %d\n", currentProcess->pid);

    if (currentProcess->remaining_time > TIME_QUANTUM) {
        currentProcess->remaining_time -= TIME_QUANTUM;
        printf("Process %d used %d units, remaining time: %d\n", currentProcess->pid, TIME_QUANTUM, currentProcess->remaining_time);
        currentProcess->queue_level = 2; // Move to Queue 2
        insertProcess(&queue2, currentProcess);
    } else {
        printf("Process %d completed in Queue 1\n", currentProcess->pid);
        free(currentProcess);
    }
}

void scheduleQueue2() {
    if (queue2 == NULL) {
        return;
    }

    ProcessPtr currentProcess = removeProcess(&queue2);
    printf("Queue 2 (FCFS): Running process %d\n", currentProcess->pid);

    // Run the process for a time quantum or until it completes
    if (currentProcess->remaining_time > TIME_QUANTUM) {
        currentProcess->remaining_time -= TIME_QUANTUM;
        printf("Process %d used %d units, remaining time: %d\n", currentProcess->pid, TIME_QUANTUM, currentProcess->remaining_time);
        currentProcess->queue_level = 3; // Move to Queue 3
        insertProcess(&queue3, currentProcess);
    } else {
        printf("Process %d completed with FCFS\n", currentProcess->pid);
        free(currentProcess);
    }
}

void scheduleQueue3() {
    if (queue3 == NULL) {
        return;
    }

    ProcessPtr shortestJob = queue3;
    ProcessPtr prev = NULL, temp = queue3;

    while (temp->next != NULL) {
        if (temp->next->remaining_time < shortestJob->remaining_time) {
            prev = temp;
            shortestJob = temp->next;
        }
        temp = temp->next;
    }

    if (prev != NULL) {
        prev->next = shortestJob->next;
    } else {
        queue3 = queue3->next;
    }

    printf("Queue 3 (SJN): Running process %d\n", shortestJob->pid);
    printf("Process %d completed with SJN\n", shortestJob->pid);
    free(shortestJob);
}

void scheduleMLFQ() {
    printf("\nProcess i:\n");
    
    scheduleQueue1();
    scheduleQueue2();
    scheduleQueue3();
    
    printf("--\n\n");
}

int main() {
    insertProcess(&queue1, createProcess(1, 0, 20));
    insertProcess(&queue1, createProcess(2, 0, 5));
    insertProcess(&queue1, createProcess(3, 0, 8));
    insertProcess(&queue1, createProcess(4, 0, 15));
    insertProcess(&queue1, createProcess(5, 0, 3));

    for (int i = 0; i < 5; i++) {
        scheduleMLFQ();
    }

    return 0;
}
