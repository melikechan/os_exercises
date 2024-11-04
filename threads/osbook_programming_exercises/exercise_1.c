#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    int n;
} thread_args;

double average(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }
    return sum / n;
}

double min_val(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    int min = array[0];

    for (int i = 1; i < n; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }

    return min;
}

double max_val(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    int max = array[0];

    for (int i = 1; i < n; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }

    return max;
}

int main(void)
{
    pthread_t thread1, thread2, thread3;

    double result1, result2, result3;
    int num_elements;

    scanf("%d", &num_elements);

    int array[num_elements];

    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &array[i]);
    }

    thread_args args = {array, num_elements};

    pthread_create(&thread1, NULL, (void *)average, (void *)&args);
    pthread_create(&thread2, NULL, (void *)min_val, (void *)&args);
    pthread_create(&thread3, NULL, (void *)max_val, (void *)&args);

    pthread_join(thread1, (void *)&result1);
    pthread_join(thread2, (void *)&result2);
    pthread_join(thread3, (void *)&result3);

    printf("Average: %.2lf\n", result1);
    printf("Min: %.2lf\n", result2);
    printf("Max: %.2lf\n", result3);

    return 0;
}