#include "kernel/types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int i, x;
    printf("testing...%d\n", getpid());
    for(i = 1; i < 1000000; i++){
        x = x + i;
    }
    printf("test ended.");
    exit(0);
    return 0;
}