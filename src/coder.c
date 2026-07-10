#include "codexion.h"

void *coder_routine(void *arg)
{
    void *ret = NULL;
    // use arg to get the coder index and sim pointer
    int coder_index = *(int *)arg;
    coder_index++;

    return ret;
}