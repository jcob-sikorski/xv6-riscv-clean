#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("XV6_TEST_OUTPUT: this should print\n");
    volatile char *p = (char *)0;

    *p; //page fault trap 14 should occur here
    printf("XV6_TEST_OUTPUT: this shouldn't print\n");
    exit(0);
}