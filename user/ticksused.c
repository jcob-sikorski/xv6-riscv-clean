#include "types.h"
#include "param.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2){
        printf("usage: mytest counter");
        exit(1);
    }

    int i, x;

    for(i = 1; i < atoi(argv[1]); i++){
        x = x + i;
    }
    
    exit(0);
    return 0;
}