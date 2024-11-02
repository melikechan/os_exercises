#include <stdio.h>
#include <pthread.h>

#define MAX 12

// Function that calculates the n! (n factorial)
int factorial(int n)
{
    if (n <= 1)
    {
        return 1; // Base case
    }

    return n * factorial(n - 1); // Recursive call
}

int main(void)
{
    pthread_t threads[MAX]; // Array to hold thread IDs

    for (int i = 0; i < MAX; i++)
    {
        // First argument -> thread ID, Second argument -> NULL (default attributes), Third argument -> function to be executed by the thread, Fourth argument -> argument to be passed to the function
        pthread_create(&threads[i], NULL, (void *)factorial, (void *)i); // Create a new thread and invoke the factorial function with i as the argument
    }

    for (int i = 0; i < MAX; i++)
    {
        int result;
        pthread_join(threads[i], (void **)&result); // Wait for the thread to finish (first argument thread, second argument the return value of the thread)
        printf("%d! = %d\n", i, result);            // Print the result
    }

    return 0;
}