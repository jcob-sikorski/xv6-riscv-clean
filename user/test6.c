#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"

/*testing good arguments for munprotect*/
int main(int argc, char *argv[])
{
    int ret = munprotect((void*)4096, 1);
    printf("XV6_TEST_OUTPUT: Return value of munprotect with len 1: %d\n", ret);

    ret = munprotect((void*)4096, 2);
    printf("XV6_TEST_OUTPUT: Return value of munprotect with len 2: %d\n", ret);
    exit(0);
}