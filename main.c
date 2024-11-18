#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10  // Size of the circular buffer

// Shared buffer and control variables
int buffer[BUFFER_SIZE];
int count = 0;  // Tracks the number of items in the buffer
int in = 0;     // Index for the producer to insert items
int out = 0;    // Index for the consumer to remove items

// Semaphores for synchronization
sem_t empty, full;  // 'empty' tracks available slots in the buffer, 'full' tracks items in the buffer

// Mutex for mutual exclusion when accessing the buffer
pthread_mutex_t mutex;

// Function to generate a random odd number between 31 and 10001
int produce_odd() {
    return (rand() % 5000) * 2 + 31;  // Generates odd numbers between 31 and 10001
}

// Producer thread function
void* producer(void* arg) {
    while (1) {
        int item = produce_odd();  // Produce a random odd number
        
        sem_wait(&empty);  // Wait if the buffer is full (i.e., no empty slots)
        pthread_mutex_lock(&mutex);  // Lock the buffer for exclusive access
        
        buffer[in] = item;  // Place the produced item into the buffer
        in = (in + 1) % BUFFER_SIZE;  // Move the 'in' index to the next position, wrapping around using modulo
        count++;  // Increment the item count in the buffer
        
        // Print the produced item and current buffer state
        printf("Produced: %d | Buffer Count: %d\n", item, count);
        
        pthread_mutex_unlock(&mutex);  // Unlock the buffer
        sem_post(&full);  // Signal that an item is available for consumption
        
        sleep(rand() % 2);  // Sleep for a random period to simulate production time
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);  // Wait if the buffer is empty (i.e., no items to consume)
        pthread_mutex_lock(&mutex);  // Lock the buffer for exclusive access
        
        int item = buffer[out];  // Consume an item from the buffer
        out = (out + 1) % BUFFER_SIZE;  // Move the 'out' index to the next position, wrapping around using modulo
        count--;  // Decrement the item count in the buffer
        
        // Print the consumed item and current buffer state
        printf("Consumed: %d | Buffer Count: %d\n", item, count);
        
        pthread_mutex_unlock(&mutex);  // Unlock the buffer
        sem_post(&empty);  // Signal that a slot is available for production
        
        sleep(rand() % 2);  // Sleep for a random period to simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // 'empty' starts with BUFFER_SIZE slots available
    sem_init(&full, 0, 0);  // 'full' starts with 0 items in the buffer
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Create the producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    // Wait for the threads to finish (the program runs indefinitely, so it never really finishes)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Cleanup resources (not reached due to infinite loop)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

