#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    // Create a pipe
    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("pipe"); // print error message
        exit(1);
    }

    pid_t reader = fork();

    if (reader < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (reader == 0)
    {
        close(fd[1]);

        printf("Now reading from child\n");

        char buf[128];

        while (read(fd[0], buf, sizeof(buf)) > 0)
        {
            printf("Child process read: %s\n", buf);
        }

        close(fd[0]);

        printf("Child process finished\n");
    }
    else
    {
        close(fd[0]);

        printf("Now writing from parent\n");

        // For passing messages (char array), strlen() is heavily recommended to use avoid the bugs.
        // For variables, sizeof() is recommended to use.
        write(fd[1], "Hello, World!\n", strlen("Hello, World!\n"));
        write(fd[1], "Morning!\n", strlen("Morning!\n"));
        write(fd[1], "Goodbye, World!", strlen("Goodbye, World!"));

        close(fd[1]);

        printf("Waiting until child process finishes...\n");
        
        wait(NULL);

        printf("Parent process finished\n");
    }
    return 0;
}