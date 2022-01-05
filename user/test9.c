#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include <stdint.h>

/*testing whether munprotect undoes the action of mprotect*/
int main(int argc, char *argv[])
{
    *(char**)0x1000 = "this should not cause a pagefault";

    int ret = mprotect((int*)4096, 1);
    printf("XV6_TEST_OUTPUT: Return value of mprotect : %d\n", ret);

    ret = munprotect((int*)4096, 1);
    printf("XV6_TEST_OUTPUT: Return value of munprotect : %d\n", ret);

    *(char**)0x1000 = "this should not cause a pagefault";

    printf("XV6_TEST_OUTPUT: Written to 0x1000\n");

    exit(0);
}