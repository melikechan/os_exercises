# Shared memory usage with C (POSIX IPC)

This folder contains an example to how to use shared memory IPC with C.

## How to run

1. To create a shared memory segment and write to it, we will run [shared_mem_writer.c](shared_mem_writer.c) first. To compile and run it, use the following commands:

```bash
gcc shared_mem_writer.c -o writer.out
./writer.out
```

2. After running the writer, we will run [shared_mem_reader.c](shared_mem_reader.c) to read from the shared memory segment. To compile and run it, use the following commands:

```bash
gcc shared_mem_reader.c -o reader.out
./reader.out
```

**Note:** Reader deletes the shared memory segment after reading from it. If you want to keep the shared memory segment, comment the `shm_unlink` call at the end.

### Functions

- `shm_open()`: Creates/opens a shared memory object. (name comes from "**sh**ared **m**emory **open**")
    Parameters:
  - `name`: Name of the shared memory object.
  - `oflag`: Flags that determine the operation of the call.
    - `O_CREAT`: Create the shared memory object if it does not exist.
    - `O_EXCL`: Ensure that this call creates the shared memory object. (If the shared memory object already exists, the call fails.)
    - `O_RDWR`: Open the shared memory object for read-write access.
    - `O_RDONLY`: Open the shared memory object for read-only access.
  - `mode`: Permissions to be applied to the shared memory object.
- `ftruncate()`: Truncates the file (shared memory object) to a specified length.
  Parameters:
  - `fd`: File descriptor of the shared memory object.
  - `length`: Length to truncate the file to. (in bytes)
- `mmap()`: Maps the shared memory object into the calling process's virtual address space.
  Parameters:
  - `addr`: Address at which to start the mapping. (Usually `NULL`, which means kernel chooses the address.)
  - `length`: Length of the mapping.
  - `prot`: Desired memory protection of the mapping.
    - `PROT_READ`: Pages can be read.
    - `PROT_WRITE`: Pages can be written.
    - `PROT_EXEC`: Pages can be executed.
    - `PROT_NONE`: Pages cannot be accessed.
  - `flags`: Determines whether updates to the mapping are visible to other processes mapping the same region.
    - `MAP_SHARED`: Updates are visible to other processes mapping the same region.
    - `MAP_PRIVATE`: Updates are not visible to other processes mapping the same region.
  - `fd`: File descriptor of the shared memory object.
  - `offset`: Offset from the beginning of the shared memory object.
- `munmap()`: Unmaps the shared memory object from the calling process's virtual address space.
  Parameters:
  - `addr`: Address of the mapping.
  - `length`: Length of the mapping.
- `close()`: Closes the file descriptor.
- `shm_unlink()`: Removes a shared memory object.
