#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 10
#define MAX_NUMBER 40

typedef struct
{
    int lower_bound;
    int upper_bound;
} fibonacci_args;

// Function that calculates the nth Fibonacci number
int fibonacci(int n)
{
    if (n <= 1)
    {
        return n; // Base case
    }

    return fibonacci(n - 1) + fibonacci(n - 2); // Recursive call
}

void *fibonacci_wrapper(void *args)
{
    fibonacci_args *f_args = (fibonacci_args *)args;
    int lower_bound = f_args->lower_bound;
    int upper_bound = f_args->upper_bound;

    int *results = malloc(sizeof(int) * (upper_bound - lower_bound));

    for (int i = lower_bound; i < upper_bound; i++)
    {
        results[i - lower_bound] = fibonacci(i);
    }

    free(f_args); // Free the allocated memory for arguments

    return (void *)results;
}

int main(void)
{
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        fibonacci_args *args = malloc(sizeof(fibonacci_args));
        args->lower_bound = i * (MAX_NUMBER / THREADS);
        args->upper_bound = (i + 1) * (MAX_NUMBER / THREADS);

        // Create a new thread and invoke the fibonacci_wrapper function with args as the argument
        pthread_create(&threads[i], NULL, fibonacci_wrapper, (void *)args);
    }

    for (int i = 0; i < THREADS; i++)
    {
        int *results;
        pthread_join(threads[i], (void **)&results);

        for (int j = 0; j < (MAX_NUMBER / THREADS); j++)
        {
            printf("Fibonacci(%d) = %d\n", i * (MAX_NUMBER / THREADS) + j, results[j]);
        }
    }

    return 0;
}