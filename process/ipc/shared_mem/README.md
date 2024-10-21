# Shared Memory IPC

Shared memory is a memory segment that is shared between **multiple processes**.

In this example, we have two programs in `system-v` and `posix` folders: `shared_mem_writer.c` and `shared_mem_reader.c`. The writer program creates a shared memory segment and writes a message to it. The reader program reads the message from the shared memory segment.

## Seeing current shared memory segments

You can see the current shared memory segments using the `ipcs` command. To see the shared memory segments, run:

```bash
ipcs -m
```
