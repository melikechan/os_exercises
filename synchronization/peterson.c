#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared variables
int turn;    // Whichever thread's turn it is to enter the critical section
int flag[2]; // Whether or not a thread wants to enter the critical section

void *thread_0(void *args)
{
    while (1)
    {
        flag[0] = 1;
        turn = 1;
        while (flag[1] == 1 && turn == 1)
            ; // Wait until it's thread 0's turn

        // Critical section
        printf("Thread 0 in critical section\n");
        flag[0] = 0;
        // Remainder section
    }
}

void *thread_1(void *args)
{
    while (1)
    {
        flag[1] = 1;
        turn = 0;
        while (flag[0] == 1 && turn == 0)
            ; // Wait until it's thread 1's turn

        // Critical section
        printf("Thread 1 in critical section\n");
        flag[1] = 0;
        // Remainder section
    }
}

int main(void)
{
    pthread_t t0, t1;

    // Initialize shared variables
    turn = 0;
    flag[0] = 0;
    flag[1] = 0;

    // Create threads
    pthread_create(&t0, NULL, thread_0, NULL);
    pthread_create(&t1, NULL, thread_1, NULL);

    // Wait for threads to finish
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    return 0;
}