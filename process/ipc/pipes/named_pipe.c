#include <fcntl.h> // O_RDONLY, O_WRONLY
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    // Create a named pipe
    char *pipe_name = "named_pipe"; // it is a file which is created in the current directory

    // Also note, if the pipe file already exists, mkfifo will fail with EEXIST
    if (mkfifo(pipe_name, 0666) < 0) // FIFO is a special file with name pipe_name
    {
        perror("mkfifo");
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
        int fd = open(pipe_name, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            exit(1);
        }

        printf("Now reading from child\n");

        char buf[128];

        while (read(fd, buf, sizeof(buf)) > 0)
        {
            printf("Child process read: %s\n", buf);

            // Clear the buffer
            // When I didn't clear, the buffer printed previous message and current message in a blended way.
            memset(buf, 0, sizeof(buf));
        }

        close(fd);

        printf("Child process finished\n");
    }
    else
    {
        int fd = open(pipe_name, O_WRONLY);
        if (fd < 0)
        {
            perror("open");
            exit(1);
        }

        printf("Now writing from parent\n");

        // For passing messages (char array), strlen() is heavily recommended to use avoid the bugs.
        // For variables, sizeof() is recommended to use.
        write(fd, "Hello, World!\n", strlen("Hello, World!\n"));
        write(fd, "Morning!\n", strlen("Morning!\n"));
        write(fd, "Goodbye, World!", strlen("Goodbye, World!"));

        close(fd);

        printf("Waiting until child process finishes...\n");

        wait(NULL);

        printf("Parent process finished\n");

        // Remove the named pipe
        if (unlink(pipe_name) < 0)
        {
            perror("unlink");
            exit(1);
        }
    }

    return 0;
}