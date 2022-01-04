#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"

/*testing bad arguments for munprotect*/
int main(int argc, char *argv[])
{
    int ret = munprotect((void*)4096, 0);
    printf("XV6_TEST_OUTPUT: Return value of munprotect with len 0: %d\n", ret);

    ret = munprotect((void*)4096, -1);
    printf("XV6_TEST_OUTPUT: Return value of munprotect with len -1: %d\n", ret);

    ret = munprotect((void*)5000, 1);
    printf("XV6_TEST_OUTPUT: Return value of munprotect with unaligned Page Address: %d\n", ret);
    exit(0);
}