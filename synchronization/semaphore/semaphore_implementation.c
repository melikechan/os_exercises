/**
 * @file semaphore_implementation.c
 * @brief Custom implementation of counting semaphores using signals
 * 
 * This program demonstrates a custom implementation of counting semaphores
 * using POSIX signals for process synchronization. The implementation includes
 * proper process management, signal handling, and memory management.
 * 
 * Note: This is an educational implementation. For production use,
 * prefer the standard POSIX semaphore implementation (sem_t).
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/** Maximum number of processes that can wait on the semaphore */
#define MAX_WAITING_PROCESSES 100

/** Initial value for the semaphore in the example */
#define INITIAL_SEM_VALUE 2

/**
 * @brief Custom semaphore structure
 * 
 * Implements a counting semaphore with a waiting list of processes.
 * Uses volatile qualifiers for variables that may be modified by signal handlers.
 */
typedef struct {
    volatile int val;           /**< Current value of the semaphore */
    volatile int waiting_count; /**< Number of processes currently waiting */
    pid_t* list;               /**< Array of waiting process IDs */
    size_t list_capacity;      /**< Maximum capacity of the waiting list */
} my_sem_t;

/** Global semaphore instance for the example */
static my_sem_t* global_sem = NULL;

/**
 * @brief Signal handler for waking up waiting processes
 * 
 * Handles SIGUSR1 signals used to wake up processes that were
 * waiting on the semaphore.
 * 
 * @param signo The signal number received
 */
static void wake_handler(int signo) {
    if (signo == SIGUSR1) {
        // Just wake up from pause()
        return;
    }
}

/**
 * @brief Initialize a new semaphore
 * 
 * Allocates and initializes a new semaphore structure with the given
 * initial value. Also allocates the waiting list.
 * 
 * @param initial_value The starting value for the semaphore
 * @return my_sem_t* Pointer to the new semaphore, or NULL on error
 */
my_sem_t* custom_sem_init(int initial_value) {
    my_sem_t* sem = malloc(sizeof(my_sem_t));
    if (!sem) {
        perror("Failed to allocate semaphore");
        return NULL;
    }

    sem->list = malloc(MAX_WAITING_PROCESSES * sizeof(pid_t));
    if (!sem->list) {
        perror("Failed to allocate waiting list");
        free(sem);
        return NULL;
    }

    sem->val = initial_value;
    sem->waiting_count = 0;
    sem->list_capacity = MAX_WAITING_PROCESSES;

    return sem;
}

/**
 * @brief Clean up a semaphore
 * 
 * Frees all memory associated with the semaphore.
 * 
 * @param sem Pointer to the semaphore to destroy
 */
void custom_sem_destroy(my_sem_t* sem) {
    if (sem) {
        free(sem->list);
        free(sem);
    }
}

/**
 * @brief Perform the wait (P) operation on a semaphore
 * 
 * Decrements the semaphore value and blocks if it becomes negative.
 * The calling process is added to the waiting list if it needs to block.
 * 
 * @param sem Pointer to the semaphore
 * @return bool true on success, false on error
 */
bool custom_wait(my_sem_t* sem) {
    if (!sem) {
        fprintf(stderr, "Invalid semaphore\n");
        return false;
    }

    sem->val--;

    if (sem->val < 0) {
        if (sem->waiting_count >= sem->list_capacity) {
            fprintf(stderr, "Too many waiting processes\n");
            sem->val++;
            return false;
        }

        pid_t calling_proc = getpid();
        sem->list[sem->waiting_count++] = calling_proc;

        // Wait for signal
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        
        // Atomically unblock SIGUSR1 and wait for it
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        pause();
        sigprocmask(SIG_BLOCK, &mask, NULL);
    }

    return true;
}

/**
 * @brief Perform the signal (V) operation on a semaphore
 * 
 * Increments the semaphore value and wakes up a waiting process if any.
 * 
 * @param sem Pointer to the semaphore
 * @return bool true on success, false on error
 */
bool custom_signal(my_sem_t* sem) {
    if (!sem) {
        fprintf(stderr, "Invalid semaphore\n");
        return false;
    }

    sem->val++;

    if (sem->val <= 0 && sem->waiting_count > 0) {
        sem->waiting_count--;
        pid_t waken_proc = sem->list[sem->waiting_count];

        if (kill(waken_proc, SIGUSR1) == -1) {
            perror("Failed to wake process");
            return false;
        }
    }

    return true;
}

/**
 * @brief Main function demonstrating semaphore usage
 * 
 * Creates multiple child processes that compete for the semaphore.
 * Each process attempts to acquire the semaphore, performs some work,
 * and then releases it.
 * 
 * @return int Exit status (0 on success, 1 on error)
 */
int main(void) {
    // Set up signal handler
    struct sigaction sa;
    sa.sa_handler = wake_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Failed to set up signal handler");
        return 1;
    }

    // Initialize semaphore
    global_sem = custom_sem_init(INITIAL_SEM_VALUE);
    if (!global_sem) {
        return 1;
    }

    printf("Initial semaphore value is %d\n", global_sem->val);

    // Test the semaphore with multiple processes
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("Fork failed");
            custom_sem_destroy(global_sem);
            return 1;
        }
        
        if (pid == 0) { // Child process
            printf("Process %d (PID: %d) trying to acquire semaphore\n", 
                   i, getpid());
            
            if (custom_wait(global_sem)) {
                printf("Process %d (PID: %d) acquired semaphore (value: %d)\n",
                       i, getpid(), global_sem->val);
                sleep(2); // Simulate some work
                
                if (custom_signal(global_sem)) {
                    printf("Process %d (PID: %d) released semaphore (value: %d)\n",
                           i, getpid(), global_sem->val);
                }
            }
            
            exit(0);
        }
    }

    // Wait for all child processes
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    custom_sem_destroy(global_sem);
    return 0;
}