/**
 * @file factorial_pthread.c
 * @brief Implementation of factorial calculation using POSIX threads
 * 
 * This program demonstrates parallel computation of factorials using multiple threads.
 * Each thread calculates the factorial of a different number, from 0 to MAX-1.
 * The implementation uses dynamic memory allocation for thread-safe operation.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/** Maximum number to calculate factorial for */
#define MAX 12

/**
 * @brief Calculates the factorial of a given number
 * 
 * This function implements a recursive factorial calculation.
 * It allocates memory for the result to ensure thread safety.
 * 
 * @param arg Pointer to the input number (void* cast of int*)
 * @return void* Pointer to the result (dynamically allocated)
 */
void* factorial(void* arg) {
    int n = *(int*)arg;
    int* result = malloc(sizeof(int));
    if (result == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    // Base case: factorial of 0 or 1 is 1
    if (n <= 1) {
        *result = 1;
        return result;
    }

    // Recursive case: n! = n * (n-1)!
    int prev;
    void* prev_result = factorial((void*)&(int){n-1});
    if (prev_result == NULL) {
        free(result);
        return NULL;
    }
    prev = *(int*)prev_result;
    free(prev_result);
    
    *result = n * prev;
    return result;
}

/**
 * @brief Main function that creates threads and manages factorial calculations
 * 
 * Creates MAX number of threads, each calculating factorial for a different number.
 * Manages memory allocation for thread arguments and cleanup of results.
 * 
 * @return int Exit status (0 on success, 1 on error)
 */
int main(void) {
    pthread_t threads[MAX];
    int* args[MAX];

    // Create threads for factorial calculation
    for (int i = 0; i < MAX; i++) {
        // Allocate memory for thread argument
        args[i] = malloc(sizeof(int));
        if (args[i] == NULL) {
            perror("malloc failed");
            return 1;
        }
        *args[i] = i;
        
        // Create thread with error checking
        if (pthread_create(&threads[i], NULL, factorial, args[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    // Wait for all threads and collect results
    for (int i = 0; i < MAX; i++) {
        int* result;
        if (pthread_join(threads[i], (void**)&result) != 0) {
            perror("pthread_join failed");
            return 1;
        }
        printf("%d! = %d\n", i, *result);
        free(result);      // Free the result memory
        free(args[i]);     // Free the argument memory
    }

    return 0;
}