#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"

/*test mprotect and munprotect by using it on the page that main() is mapped to and attempting to write things to main.*/
int main(int argc, char *argv[])
{
    int ret = mprotect((void *)main, 1);
    printf("XV6_TEST_OUTPUT: Return value of mprotect on Page where MAIN is mapped: %d\n", ret);

    *(char**)main = (char*)0x2000; //page fault trap 14 should occur here

    printf("XV6_TEST_OUTPUT: this shouldn't print\n");
   
    exit(0);
}