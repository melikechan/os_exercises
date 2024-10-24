#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

// Timer program that measures the time elapsed on a command
int main(int argc, char *argv[])
{
    struct timespec *start, *end;

    start = mmap(NULL, sizeof(clock_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    end = mmap(NULL, sizeof(clock_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (start == MAP_FAILED || end == MAP_FAILED)
    {
        fprintf(stderr, "Memory mapping failed\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fatal error: fork() failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        clock_gettime(CLOCK_REALTIME, start);

        int total_length = 0;
        for (int i = 1; i < argc; i++)
        {
            total_length += strlen(argv[i]) + 1; // +1 for space or null terminator
        }

        // Allocate memory for the command string
        char *command = (char *)malloc(total_length * sizeof(char));
        if (command == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        // Initialize the command string
        strcpy(command, argv[1]);

        for (int i = 2; i < argc; i++)
        {
            strcat(command, " ");
            strcat(command, argv[i]);
        }

        execl("/bin/sh", "sh", "-c", command, NULL);
    }
    else
    {
        wait(NULL);
        clock_gettime(CLOCK_REALTIME, end);

        // seconds + nanoseconds
        double time_taken = ((double)(end->tv_sec - start->tv_sec) + (double)(end->tv_nsec - start->tv_nsec) / 1000000000);

        printf("Elapsed time: %f seconds\n", time_taken);
    }

    return 0;
}