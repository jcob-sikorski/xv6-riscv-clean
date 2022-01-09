#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int numframes = 50;
    int *frames = malloc(numframes*sizeof(*frames));
    memset(frames, 0, sizeof(*frames));

    dump_allocated(frames, numframes);
    for (int i = 0; i < numframes; i++) {
        printf("%d\n", *frames);
        frames++;
    }
    return 0;
}