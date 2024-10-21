#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(void)
{
    // Generate IPC key (System V IPC key)
    key_t shared_mem_ipc_key = ftok("shared_mem", 65);

    // Create shared memory
    int shared_mem_id = shmget(shared_mem_ipc_key, SHM_SIZE, IPC_CREAT | 0666);

    printf("shared_mem_id: %d\n", shared_mem_id);
    // Attach shared memory ([sh]ared [m]emory [at]tach)
    int *shared_mem_ptr = shmat(shared_mem_id, NULL, 0);

    // Check if shared memory is attached
    if (shared_mem_ptr == (int *)-1)
    {
        printf("Error\n");
        return 1;
    }

    printf("Writing to shared memory\n");

    // Write to shared memory (int array)
    for (int i = 0; i < SHM_SIZE / sizeof(int); i++)
    {
        shared_mem_ptr[i] = i;
    }

    // Detach shared memory ([sh]ared [m]emory [d]e[t]ach)
    shmdt(shared_mem_ptr);

    return 0;
}