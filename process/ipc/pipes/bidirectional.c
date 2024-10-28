
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    // Create two pipes, one for communication between parent and child, and the other for communication between child and parent
    int fd1[2], fd2[2]; // fd1: parent -> child, fd2: child -> parent
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
    {
        perror("pipe"); // print error message
        exit(1);
    }

    pid_t child = fork();

    if (child < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (child == 0)
    {
        // Close unused ends to avoid hanging
        close(fd1[1]);
        close(fd2[0]);

        printf("Now reading from child\n");

        char buf[128];

        // Read from parent
        while (read(fd1[0], buf, sizeof(buf)) > 0)
        {
            printf("Child process read: %s\n", buf);
        }

        close(fd1[0]);

        // Now child writes to parent
        printf("Now writing from child\n");
        write(fd2[1], "Hello, Parent!\n", strlen("Hello, Parent!\n"));
        write(fd2[1], "Goodbye, Parent!", strlen("Goodbye, Parent!"));

        close(fd2[1]);

        printf("Child process finished\n");
    }
    else
    {
        // Close unused ends to avoid hanging
        close(fd1[0]);
        close(fd2[1]);

        printf("Now writing from parent\n");
        // For passing messages (char array), strlen() is heavily recommended to use avoid the bugs.
        // For variables, sizeof() is recommended to use.
        write(fd1[1], "Hello, World!\n", strlen("Hello, World!\n"));
        write(fd1[1], "Morning!\n", strlen("Morning!\n"));
        write(fd1[1], "Goodbye, World!", strlen("Goodbye, World!"));

        close(fd1[1]);

        printf("Waiting until child process finishes...\n");

        wait(NULL);

        char buf[128];

        // Read from child
        while (read(fd2[0], buf, sizeof(buf)) > 0)
        {
            printf("Parent process read: %s\n", buf);
        }

        close(fd2[0]);

        printf("Parent process finished\n");
    }
    return 0;
}