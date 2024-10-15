#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 3

int canAllocate(int process, int need[][MAX_RESOURCES], int available[])
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (need[process][i] > available[i])
        {
            return 0;
        }
    }
    return 1;
}

void printMatrix(const char *name, int matrix[MAX_PROCESSES][MAX_RESOURCES], int rows)
{
    printf("%s Matrix:\n", name);
    printf("\tA\tB\tC\n");
    for (int i = 0; i < rows; i++)
    {
        printf("P%d\t", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printArray(const char *name, int array[], int size)
{
    printf("%s Array: ", name);
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int findSafeSequence(int n, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int available[])
{
    int finish[MAX_PROCESSES] = {0};
    int safeSequence[MAX_PROCESSES];
    int work[MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        work[i] = available[i];
    }

    printMatrix("Allocation", allocation, n);
    printMatrix("Maximum", max, n);
    printArray("Available", available, MAX_RESOURCES);
    printMatrix("Need", need, n);

    int count = 0;
    while (count < n)
    {
        int found = 0;
        for (int p = 0; p < n; p++)
        {
            if (!finish[p] && canAllocate(p, need, work))
            {
                for (int j = 0; j < MAX_RESOURCES; j++)
                {
                    work[j] += allocation[p][j];
                }
                safeSequence[count++] = p;
                finish[p] = 1;
                found = 1;
            }
        }
        if (!found)
        {
            printf("System is in unsafe state.\n");
            return 0;
        }
    }

    printf("System is in safe state.\nSafe sequence: ");
    for (int i = 0; i < n; i++)
    {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");
    return 1;
}

int main()
{
    int n;
    int allocation[MAX_PROCESSES][MAX_RESOURCES], max[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES], total[MAX_RESOURCES] = {10, 5, 7};

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the allocation matrix (P1 to Pn for A, B, C):\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("Enter the maximum resource matrix (P1 to Pn for A, B, C):\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        int sumAllocated = 0;
        for (int j = 0; j < n; j++)
        {
            sumAllocated += allocation[j][i];
        }
        available[i] = total[i] - sumAllocated;
    }

    if (!findSafeSequence(n, allocation, max, available))
    {
        printf("Deadlock is possible!\n");
    }

    return 0;
}
