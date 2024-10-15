#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_RESOURCES 3
#define MAX_CUSTOMERS 5

// Bank structure
typedef struct {
    int available[MAX_RESOURCES]; // Available instances of resources
    int maximum[MAX_CUSTOMERS][MAX_RESOURCES]; // Maximum resources needed by each customer
    int allocation[MAX_CUSTOMERS][MAX_RESOURCES]; // Resources currently allocated to customers
    int need[MAX_CUSTOMERS][MAX_RESOURCES]; // Remaining resources needed by customers
    pthread_mutex_t mutex; // Mutex for synchronizing access to shared data
} Bank;

Bank bank;

// Function to check if the system is in a safe state
bool is_safe_state() {
    int work[MAX_RESOURCES];
    bool finish[MAX_CUSTOMERS] = {false};
    
    // Initialize work with available resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = bank.available[i];
    }

    bool safe = false;
    while (true) {
        safe = false;
        for (int i = 0; i < MAX_CUSTOMERS; i++) {
            if (!finish[i]) {
                // Check if need can be satisfied with work
                bool can_allocate = true;
                for (int j = 0; j < MAX_RESOURCES; j++) {
                    if (bank.need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    // Simulate allocation
                    for (int j = 0; j < MAX_RESOURCES; j++) {
                        work[j] += bank.allocation[i][j];
                    }
                    finish[i] = true;
                    safe = true;
                }
            }
        }
        if (!safe) break; // No more customers can proceed
    }
    // Return true if all customers can finish
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        if (!finish[i]) return false;
    }
    return true;
}

// Request resources
bool request_resources(int customer_id, int request[]) {
    pthread_mutex_lock(&bank.mutex);

    // Check if request is less than need
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > bank.need[customer_id][i]) {
            pthread_mutex_unlock(&bank.mutex);
            return false; // Error: request more than needed
        }
    }

    // Check if request is less than available
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > bank.available[i]) {
            pthread_mutex_unlock(&bank.mutex);
            return false; // Not enough resources available
        }
    }

    // Pretend to allocate the requested resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        bank.available[i] -= request[i];
        bank.allocation[customer_id][i] += request[i];
        bank.need[customer_id][i] -= request[i];
    }

    // Check if the state is safe
    if (!is_safe_state()) {
        // Rollback the allocation
        for (int i = 0; i < MAX_RESOURCES; i++) {
            bank.available[i] += request[i];
            bank.allocation[customer_id][i] -= request[i];
            bank.need[customer_id][i] += request[i];
        }
        pthread_mutex_unlock(&bank.mutex);
        return false; // Not a safe state, request denied
    }

    pthread_mutex_unlock(&bank.mutex);
    return true; // Request granted
}

// Release resources
void release_resources(int customer_id, int release[]) {
    pthread_mutex_lock(&bank.mutex);

    for (int i = 0; i < MAX_RESOURCES; i++) {
        bank.available[i] += release[i];
        bank.allocation[customer_id][i] -= release[i];
        bank.need[customer_id][i] += release[i];
    }

    pthread_mutex_unlock(&bank.mutex);
}

// Thread function simulating a customer
void* customer_thread(void* arg) {
    int customer_id = *(int*)arg;
    int request[MAX_RESOURCES];
    for (int i = 0; i < MAX_RESOURCES; i++) {
        request[i] = rand() % (bank.need[customer_id][i] + 1);
    }
    printf("Customer %d requests: ", customer_id);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("%d ", request[i]);
    }
    printf("\n");

    if (request_resources(customer_id, request)) {
        printf("Customer %d allocated resources: ", customer_id);
        for (int i = 0; i < MAX_RESOURCES; i++) {
            printf("%d ", request[i]);
        }
        printf("\n");
        // Simulate using the resources
        sleep(rand() % 3 + 1);
        // Release resources
        release_resources(customer_id, request);
        printf("Customer %d released resources.\n", customer_id);
    } else {
        printf("Customer %d request denied.\n", customer_id);
    }

    return NULL;
}

int main() {
    pthread_t customers[MAX_CUSTOMERS];
    int customer_ids[MAX_CUSTOMERS];

    // Initialize bank
    pthread_mutex_init(&bank.mutex, NULL);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        bank.available[i] = 10; // Total resources available
    }

    // Initialize maximum resources, allocation, and need
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customer_ids[i] = i;
        for (int j = 0; j < MAX_RESOURCES; j++) {
            bank.maximum[i][j] = rand() % 5 + 1; // Random max demand
            bank.allocation[i][j] = 0; // Initially no resources allocated
            bank.need[i][j] = bank.maximum[i][j]; // Initially need is equal to max
        }
    }

    // Create customer threads
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_create(&customers[i], NULL, customer_thread, &customer_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&bank.mutex);
    return 0;
}
