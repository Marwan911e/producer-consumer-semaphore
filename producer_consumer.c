#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 10000
#define MAX_VALUE 1000

// Global variables
int buffer[ARRAY_SIZE];
int producerIndex = 0;
int consumerIndex = 0;
int producedCount = 0;
int consumedCount = 0;
int producerFinished = 0;

// Synchronization primitives
pthread_mutex_t mutex;
sem_t notFull;   // Semaphore for buffer not full
sem_t notEmpty;  // Semaphore for buffer not empty

// File pointers
FILE *producerFile;
FILE *consumerFile;

// Producer thread function
void* producer(void* arg) {
    (void)arg; // Suppress unused parameter warning
    // Initialize random number generator
    srand(time(NULL) + pthread_self());
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        // Generate random number between 0 and 1000
        int value = rand() % (MAX_VALUE + 1);
        
        // Wait for buffer not full (semaphore)
        sem_wait(&notFull);
        
        // Enter critical section
        pthread_mutex_lock(&mutex);
        
        // Write to buffer
        buffer[producerIndex % ARRAY_SIZE] = value;
        producerIndex++;
        producedCount++;
        
        // Write to file
        if (producerFile != NULL) {
            fprintf(producerFile, "Producer produced item number %d with the value %d\n", 
                    i + 1, value);
            fflush(producerFile); // Ensure immediate writing
        }
        
        // Exit critical section
        pthread_mutex_unlock(&mutex);
        
        // Signal buffer not empty
        sem_post(&notEmpty);
        
        // Small delay to simulate work
        usleep(1000); // 1 millisecond
    }
    
    // Signal that producer is finished
    pthread_mutex_lock(&mutex);
    producerFinished = 1;
    pthread_mutex_unlock(&mutex);
    
    // Wake up consumer if it's waiting
    sem_post(&notEmpty);
    
    printf("Producer finished. Produced %d items.\n", producedCount);
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    (void)arg; // Suppress unused parameter warning
    while (1) {
        // Wait for buffer not empty (semaphore)
        sem_wait(&notEmpty);
        
        // Enter critical section
        pthread_mutex_lock(&mutex);
        
        // Check if producer is finished and buffer is empty
        if (producerFinished && producerIndex <= consumerIndex) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        // Read from buffer
        int value = buffer[consumerIndex % ARRAY_SIZE];
        consumerIndex++;
        consumedCount++;
        
        // Write to screen
        printf("Consumer consumed item number %d with the value %d\n", 
               consumedCount, value);
        
        // Write to file
        if (consumerFile != NULL) {
            fprintf(consumerFile, "Consumer consumed item number %d with the value %d\n", 
                    consumedCount, value);
            fflush(consumerFile); // Ensure immediate writing
        }
        
        // Exit critical section
        pthread_mutex_unlock(&mutex);
        
        // Signal buffer not full
        sem_post(&notFull);
        
        // Small delay to simulate work
        usleep(1000); // 1 millisecond
    }
    
    printf("Consumer finished. Consumed %d items.\n", consumedCount);
    return NULL;
}

int main() {
    pthread_t producerThread, consumerThread;
    
    printf("Starting Producer-Consumer program...\n");
    printf("Array size: %d\n", ARRAY_SIZE);
    printf("Producer will generate %d random numbers (0-%d)\n", ARRAY_SIZE, MAX_VALUE);
    printf("Consumer will read and display all values\n");
    printf("Output files: Producer_activity.txt and Consumer_activity.txt\n");
    printf("----------------------------------------\n");
    
    // Initialize synchronization primitives
    pthread_mutex_init(&mutex, NULL);
    sem_init(&notFull, 0, ARRAY_SIZE);   // Initially buffer is empty, so notFull = ARRAY_SIZE
    sem_init(&notEmpty, 0, 0);           // Initially buffer is empty, so notEmpty = 0
    
    // Open files for writing
    producerFile = fopen("Producer_activity.txt", "w");
    consumerFile = fopen("Consumer_activity.txt", "w");
    
    if (producerFile == NULL) {
        fprintf(stderr, "Error: Could not open Producer_activity.txt\n");
        return 1;
    }
    if (consumerFile == NULL) {
        fprintf(stderr, "Error: Could not open Consumer_activity.txt\n");
        fclose(producerFile);
        return 1;
    }
    
    // Create threads
    if (pthread_create(&producerThread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "Error: Could not create producer thread\n");
        return 1;
    }
    
    if (pthread_create(&consumerThread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Error: Could not create consumer thread\n");
        return 1;
    }
    
    // Wait for threads to complete
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
    
    // Clean up
    fclose(producerFile);
    fclose(consumerFile);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&notFull);
    sem_destroy(&notEmpty);
    
    printf("----------------------------------------\n");
    printf("Program completed successfully!\n");
    printf("Total items produced: %d\n", producedCount);
    printf("Total items consumed: %d\n", consumedCount);
    
    return 0;
}
