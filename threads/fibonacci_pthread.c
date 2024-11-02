#include <stdio.h>
#include <pthread.h>

#define MAX 12

// Function that calculates the nth Fibonacci number
int fibonacci(int n)
{
    if (n <= 1)
    {
        return n; // Base case
    }

    return fibonacci(n - 1) + fibonacci(n - 2); // Recursive call
}

int main(void)
{
    pthread_t threads[MAX]; // Array to hold thread IDs

    for (int i = 0; i < MAX; i++)
    {
        // First argument -> thread ID, Second argument -> NULL (default attributes), Third argument -> function to be executed by the thread, Fourth argument -> argument to be passed to the function
        pthread_create(&threads[i], NULL, (void *)fibonacci, (void *)i); // Create a new thread and invoke the fibonacci function with i as the argument
    }

    for (int i = 0; i < MAX; i++)
    {
        int result;
        pthread_join(threads[i], (void **)&result); // Wait for the thread to finish (first argument thread, second argument the return value of the thread)
        printf("Fibonacci(%d) = %d\n", i, result);  // Print the result
    }

    return 0;
}