#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    int n;
} thread_args;

void *average(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    double *result = malloc(sizeof(double));
    *result = 0;

    for (int i = 0; i < n; i++)
    {
        *result += array[i];
    }
    *result /= n;

    return (void *)result;
}

void *min_val(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    double *result = malloc(sizeof(double));
    *result = array[0];

    for (int i = 1; i < n; i++)
    {
        if (array[i] < *result)
        {
            *result = array[i];
        }
    }

    return (void *)result;
}

void *max_val(void *args)
{
    thread_args *t_args = (thread_args *)args;
    int *array = t_args->array;
    int n = t_args->n;

    double *result = malloc(sizeof(double));
    *result = array[0];

    for (int i = 1; i < n; i++)
    {
        if (array[i] > *result)
        {
            *result = array[i];
        }
    }

    return (void *)result;
}

int main(void)
{
    pthread_t thread1, thread2, thread3;
    double *result1, *result2, *result3;
    int num_elements;

    printf("Enter number of elements: ");
    scanf("%d", &num_elements);

    int *array = malloc(num_elements * sizeof(int));

    printf("Enter elements: ");
    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &array[i]);
    }

    thread_args args = {array, num_elements};

    pthread_create(&thread1, NULL, average, (void *)&args);
    pthread_create(&thread2, NULL, min_val, (void *)&args);
    pthread_create(&thread3, NULL, max_val, (void *)&args);

    pthread_join(thread1, (void **)&result1);
    pthread_join(thread2, (void **)&result2);
    pthread_join(thread3, (void **)&result3);

    printf("Average: %lf\n", *result1);
    printf("Min: %lf\n", *result2);
    printf("Max: %lf\n", *result3);

    free(result1);
    free(result2);
    free(result3);
    free(array);

    return 0;
}
