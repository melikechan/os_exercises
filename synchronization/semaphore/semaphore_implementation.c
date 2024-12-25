#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <semaphore.h>

typedef struct
{
    int val;     // value of the semaphore
    pid_t *list; // list of processes waiting
} my_sem_t;

void custom_wait(my_sem_t *sem)
{
    sem->val--;

    if (sem->val < 0) // Wait until a lock is available
    {
        int count_cur_waiting = sizeof(sem->list) / sizeof(my_sem_t);
        pid_t calling_proc = getpid();
        sem->list = realloc(sem->list, (count_cur_waiting + sizeof(my_sem_t)));

        sem->list[count_cur_waiting] = calling_proc;
        pause();
    }
}

void custom_signal(my_sem_t *sem)
{
    sem->val++;
    if (sem->val <= 0) // Hand it to another process that demands it but sleeps... (Zzz)
    {
        int count_cur_waiting = sizeof(sem->list) / sizeof(my_sem_t);
        pid_t waken_proc = sem->list[count_cur_waiting - 1];

        free(&sem->list[count_cur_waiting - 1]);
        sem->list = realloc(sem->list, (count_cur_waiting - sizeof(my_sem_t)));
        kill(waken_proc, SIGUSR1); // Send a signal to wake the process up
    }
}

// Actually, using different processes would be way better
int main(void)
{
    my_sem_t *sem = malloc(sizeof(my_sem_t));
    sem->val = 2;

    printf("initial semaphore value is %d\n", sem->val);
    custom_wait(sem);
    printf("waited, current semaphore value is: %d\n", sem->val);
    custom_wait(sem);
    printf("waited, current semaphore value is: %d\n", sem->val);
    custom_signal(sem);
    printf("signaled, current semaphore value is %d\n", sem->val);
    custom_signal(sem);
    printf("signaled, current semaphore value is %d\n", sem->val);

    return 0;
}