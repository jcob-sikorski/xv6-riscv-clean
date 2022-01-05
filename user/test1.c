#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("XV6_TEST_OUTPUT %d\n", (int*)main >= 0x1000);
    exit(0);
}
