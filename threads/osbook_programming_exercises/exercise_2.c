#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void sieve(void *args)
{
    int upper_bound = *((int *)args);

    int *is_prime = (int *)malloc((upper_bound + 1) * sizeof(int));

    for (int i = 0; i <= upper_bound; i++)
    {
        is_prime[i] = 1;
    }

    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i * i <= upper_bound; i++)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= upper_bound; j += i)
            {
                is_prime[j] = 0;
            }
        }
    }

    for (int i = 0; i <= upper_bound; i++)
    {
        if (is_prime[i])
        {
            printf("%d\n", i);
        }
    }
}

int main(void)
{
    pthread_t thread1;
    int upper_bound;

    scanf("%d", &upper_bound);

    pthread_create(&thread1, NULL, (void *)sieve, (void *)&upper_bound); // Run the sieve function in a separate thread

    pthread_join(thread1, NULL); // Wait for the thread to finish

    return 0;
}