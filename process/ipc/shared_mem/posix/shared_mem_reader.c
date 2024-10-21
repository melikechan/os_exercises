#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(void)
{
    // Open existing shared memory object
    int shared_memobj_fd = shm_open("shared_mem", O_RDWR, 0666);

    if (shared_memobj_fd == -1)
    {
        printf("Error\n");
        return 1;
    }

    // Map shared memory object (get pointer to shared memory)
    int *shared_mem_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memobj_fd, 0);

    if (shared_mem_ptr == MAP_FAILED)
    {
        printf("Error\n");
        return 1;
    }

    // Read from shared memory (int array)
    for (int i = 0; i < SHM_SIZE / sizeof(int); i++)
    {
        printf("%d\n", shared_mem_ptr[i]);
    }

    // Unmap shared memory object
    if (munmap(shared_mem_ptr, SHM_SIZE) == -1)
    {
        printf("Error\n");
        return 1;
    }

    // Close shared memory object
    if (close(shared_memobj_fd) == -1)
    {
        printf("Error\n");
        return 1;
    }

    // Remove shared memory object
    if (shm_unlink("shared_mem") == -1)
    {
        printf("Error\n");
        return 1;
    }

    return 0;
}