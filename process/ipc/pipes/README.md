# Pipe IPC

This folder contains examples of pipe usage in C.

## How to run

There are three types of pipe examples in this folder:

- [unidirectional.c](unidirectional.c): Unidirectional unnamed pipe.
- [bidirectional.c](bidirectional.c): Bidirectional unnamed pipe.
- [named_pipe.c](named_pipe.c): Named pipe (FIFO).

To compile and run each example, use the following commands:

```bash
gcc <example-name>.c -o <example-name>.out
```

**NOTE:** Named pipe example removes the FIFO file after running. If you want to keep the FIFO file, comment the `unlink` call at the end.

## How does it work?

Pipes are one of the intercommunication mechanisms in *nix systems. There are two types of pipes:

1. **Unnamed Pipes**:

- Unnamed pipes don't have a name.
- Unnamed pipes can be used between **related processes.**
- When process terminates, the pipe is **closed automatically.**
- It is **unidirectional.**

2. **Named Pipes (FIFO)**:

- Named pipes has a name, and it is represented as a **file** in the file system.
- Named pipes can be used between **unrelated processes.**
- When one of the processes terminates, the named pipe **still exists**.
- It is **bidirectional.**

### Functions

- `pipe()`: Creates a pipe.
  Parameters:
  - `int pipefd[2]`: Array to store the file descriptors of the pipe.
    - `pipefd[0]`: File descriptor for reading.
    - `pipefd[1]`: File descriptor for writing.
  Returns:
    - `0` on success.
    - `-1` on error.
- `mkfifo()`: Creates a named pipe.
  Parameters:
  - `const char* pathname`: Pathname of the FIFO file.
  - `mode_t mode`: Permissions of the FIFO file.
  Returns:
    - `0` on success.
    - `-1` on error.
- `unlink()`: Removes the named pipe.
  Parameters:
  - `const char* pathname`: Pathname of the FIFO file.
  Returns:
    - `0` on success.
    - `-1` on error.
