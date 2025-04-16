/**
 * @file peterson.c
 * @brief Implementation of Peterson's Algorithm for mutual exclusion
 * 
 * This program demonstrates Peterson's solution to the mutual exclusion problem
 * between two threads. It includes proper synchronization mechanisms and graceful
 * shutdown handling.
 * 
 * Peterson's algorithm ensures mutual exclusion without requiring hardware atomic
 * instructions, though it relies on the memory model providing sequential consistency.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

/** Maximum number of iterations each thread will perform */
#define MAX_ITERATIONS 10

// Shared variables for Peterson's algorithm
volatile int turn;           /**< Indicates whose turn it is to enter critical section */
volatile bool flag[2];       /**< Indicates if a thread wants to enter critical section */
volatile bool running = true;/**< Control flag for graceful shutdown */

// Test variable for demonstrating mutual exclusion
volatile int shared_counter = 0;

/**
 * @brief Signal handler for graceful shutdown
 * 
 * Handles SIGINT (Ctrl+C) by setting the running flag to false,
 * allowing threads to complete their current iteration and exit.
 * 
 * @param signum The signal number received
 */
void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nShutting down gracefully...\n");
        running = false;
    }
}

/**
 * @brief Thread 0's entry point function
 * 
 * Implements Peterson's algorithm for thread 0. The thread attempts to enter
 * its critical section MAX_ITERATIONS times, unless interrupted.
 * 
 * @param args Thread arguments (unused)
 * @return void* Always returns NULL
 */
void* thread_0(void* args) {
    int iterations = 0;
    while (running && iterations < MAX_ITERATIONS) {
        // Entry section
        flag[0] = true;
        turn = 1;
        while (flag[1] && turn == 1 && running) {
            // Busy wait with small sleep to reduce CPU usage
            usleep(1);
        }

        if (!running) break;

        // Critical section
        printf("Thread 0 entering critical section (counter: %d)\n", shared_counter);
        shared_counter++;
        usleep(100000); // Simulate some work
        printf("Thread 0 leaving critical section (counter: %d)\n", shared_counter);

        // Exit section
        flag[0] = false;
        iterations++;
        
        // Remainder section
        usleep(50000); // Some non-critical work
    }
    return NULL;
}

/**
 * @brief Thread 1's entry point function
 * 
 * Implements Peterson's algorithm for thread 1. The thread attempts to enter
 * its critical section MAX_ITERATIONS times, unless interrupted.
 * 
 * @param args Thread arguments (unused)
 * @return void* Always returns NULL
 */
void* thread_1(void* args) {
    int iterations = 0;
    while (running && iterations < MAX_ITERATIONS) {
        // Entry section
        flag[1] = true;
        turn = 0;
        while (flag[0] && turn == 0 && running) {
            // Busy wait with small sleep to reduce CPU usage
            usleep(1);
        }

        if (!running) break;

        // Critical section
        printf("Thread 1 entering critical section (counter: %d)\n", shared_counter);
        shared_counter++;
        usleep(100000); // Simulate some work
        printf("Thread 1 leaving critical section (counter: %d)\n", shared_counter);

        // Exit section
        flag[1] = false;
        iterations++;
        
        // Remainder section
        usleep(50000); // Some non-critical work
    }
    return NULL;
}

/**
 * @brief Main function that sets up and manages the threads
 * 
 * Initializes the shared variables, sets up signal handling, creates the threads,
 * and waits for their completion. Also verifies the final counter value.
 * 
 * @return int Exit status (0 on success, 1 on error)
 */
int main(void) {
    pthread_t t0, t1;

    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    // Initialize shared variables
    turn = 0;
    flag[0] = false;
    flag[1] = false;
    shared_counter = 0;

    // Create threads
    if (pthread_create(&t0, NULL, thread_0, NULL) != 0) {
        perror("Failed to create thread 0");
        return 1;
    }

    if (pthread_create(&t1, NULL, thread_1, NULL) != 0) {
        perror("Failed to create thread 1");
        pthread_cancel(t0);
        return 1;
    }

    // Wait for threads to finish
    if (pthread_join(t0, NULL) != 0) {
        perror("Failed to join thread 0");
    }

    if (pthread_join(t1, NULL) != 0) {
        perror("Failed to join thread 1");
    }

    // Verify results
    printf("\nFinal counter value: %d\n", shared_counter);
    printf("Expected counter value: %d\n", MAX_ITERATIONS * 2);

    return 0;
}