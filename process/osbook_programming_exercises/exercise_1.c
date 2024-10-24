#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t zombie = fork();

    if (zombie > 0)
    // As parent sleeps, child becomes zombie (because to become zombie, child must terminate before parent)
    {
        sleep(10);
    }

    return 0;
}