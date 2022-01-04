#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include <stdint.h>

/*test that forking the process keeps the same protection bits.*/
int main(int argc, char *argv[])
{   
    char *p = (char *)0x1000;
    int ret = mprotect((uintptr_t)p, 1);
    printf("XV6_TEST_OUTPUT: Return value of mprotect on memory 0x1000: %d\n", ret);

    int f = fork();
    if(f)
    {
        printf("XV6_TEST_OUTPUT: Inside child process\n");
        *p = 'w'; //page fault trap 14 should occur here

        printf("XV6_TEST_OUTPUT: This should not be printed\n");
        exit(0);
    }
    else
    {
        (void)wait(&f);
    }
    
    exit(0);
}