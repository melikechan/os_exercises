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

    // Get shared memory
    int shared_mem_id = shmget(shared_mem_ipc_key, SHM_SIZE, 0666);

    // Attach shared memory ([sh]ared [m]emory [at]tach)
    int *shared_mem_ptr = shmat(shared_mem_id, NULL, 0);

    // Check if shared memory is attached
    if (shared_mem_ptr == (int *)-1)
    {
        printf("Error\n");
        return 1;
    }

    // Read from shared memory (int array)
    for (int i = 0; i < SHM_SIZE / sizeof(int); i++)
    {
        printf("%d\n", shared_mem_ptr[i]);
    }

    // Detach shared memory ([sh]ared [m]emory [d]e[t]ach)
    shmdt(shared_mem_ptr);

    printf("Deleting shared memory with id: %d\n", shared_mem_id);
    
    // Remove shared memory
    shmctl(shared_mem_id, IPC_RMID, NULL);

    return 0;
}