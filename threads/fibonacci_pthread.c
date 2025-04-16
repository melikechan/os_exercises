/**
 * @file fibonacci_pthread.c
 * @brief Implementation of Fibonacci sequence calculation using POSIX threads
 * 
 * This program demonstrates parallel computation of Fibonacci numbers using multiple threads.
 * The work is divided among THREADS number of threads, each calculating a range of Fibonacci numbers.
 * The implementation uses dynamic memory allocation and proper cleanup mechanisms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/** Number of threads to use for parallel computation */
#define THREADS 10

/** Maximum Fibonacci number to calculate */
#define MAX_NUMBER 40

/**
 * @brief Structure to hold arguments for thread function
 * 
 * Contains the range of Fibonacci numbers that a thread should calculate.
 */
typedef struct {
    int lower_bound;  /**< Starting number in the range (inclusive) */
    int upper_bound;  /**< Ending number in the range (exclusive) */
} fibonacci_args;

/**
 * @brief Calculates the nth Fibonacci number
 * 
 * Implements recursive Fibonacci calculation.
 * Note: This is not the most efficient implementation but serves as a good example
 * for demonstrating parallel computation.
 * 
 * @param n The position in Fibonacci sequence to calculate
 * @return int The nth Fibonacci number
 */
int fibonacci(int n) {
    if (n <= 1) {
        return n;  // Base case: F(0) = 0, F(1) = 1
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // Recursive case
}

/**
 * @brief Thread function that calculates a range of Fibonacci numbers
 * 
 * This function takes a range of numbers and calculates the Fibonacci number
 * for each position in that range. Results are stored in dynamically allocated memory.
 * 
 * @param args Pointer to fibonacci_args structure containing the range
 * @return void* Pointer to array of results (dynamically allocated)
 */
void* fibonacci_wrapper(void* args) {
    fibonacci_args* f_args = (fibonacci_args*)args;
    int lower_bound = f_args->lower_bound;
    int upper_bound = f_args->upper_bound;

    // Allocate memory for results
    int* results = malloc(sizeof(int) * (upper_bound - lower_bound));
    if (results == NULL) {
        perror("malloc failed in fibonacci_wrapper");
        free(f_args);
        return NULL;
    }

    // Calculate Fibonacci numbers for the assigned range
    for (int i = lower_bound; i < upper_bound; i++) {
        results[i - lower_bound] = fibonacci(i);
    }

    free(f_args);  // Free the arguments structure
    return results;
}

/**
 * @brief Main function that manages thread creation and result collection
 * 
 * Creates THREADS number of threads and divides the work of calculating Fibonacci numbers
 * among them. Manages memory allocation, thread creation/joining, and cleanup.
 * 
 * @return int Exit status (0 on success, 1 on error)
 */
int main(void) {
    pthread_t threads[THREADS];
    int* results[THREADS] = {NULL};

    // Create threads and assign work ranges
    for (int i = 0; i < THREADS; i++) {
        // Allocate and initialize thread arguments
        fibonacci_args* args = malloc(sizeof(fibonacci_args));
        if (args == NULL) {
            perror("malloc failed for args");
            goto cleanup;
        }

        // Calculate range for this thread
        args->lower_bound = i * (MAX_NUMBER / THREADS);
        args->upper_bound = (i + 1) * (MAX_NUMBER / THREADS);

        // Create thread with error checking
        if (pthread_create(&threads[i], NULL, fibonacci_wrapper, args) != 0) {
            perror("pthread_create failed");
            free(args);
            goto cleanup;
        }
    }

    // Wait for threads and process results
    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], (void**)&results[i]) != 0) {
            perror("pthread_join failed");
            goto cleanup;
        }

        // Print results if available
        if (results[i] != NULL) {
            for (int j = 0; j < (MAX_NUMBER / THREADS); j++) {
                printf("Fibonacci(%d) = %d\n", 
                       i * (MAX_NUMBER / THREADS) + j, 
                       results[i][j]);
            }
        }
    }

cleanup:
    // Clean up allocated memory
    for (int i = 0; i < THREADS; i++) {
        if (results[i] != NULL) {
            free(results[i]);
        }
    }

    return 0;
}