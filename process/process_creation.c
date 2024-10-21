#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("Enter of the program\n");
    printf("Before fork\n");
    pid_t child_process = fork();
    printf("After fork\n");

    printf("Child Process: %d\n", child_process);

    if (child_process < 0)
    {
        printf("Error\n");
    }
    else if (child_process == 0)
    {
        printf("Child Process\n");
        pid_t grand_child_process = fork();
        printf("After fork of fork\n");
        printf("Grand Child Process: %d\n", grand_child_process);

        if (grand_child_process < 0)
        {
            printf("Error\n");
        }
        else if (grand_child_process == 0)
        {
            printf("Grand Child Process\n");
        }
        else
        {
            printf("Child Process\n");
        }
    }
    else
    {
        printf("Parent Process\n");
    }
    printf("End of the program\n");
    return 0;
}