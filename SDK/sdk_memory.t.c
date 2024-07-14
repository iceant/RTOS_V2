#include <sdk_memory.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    void* m100 = SDK_ALLOC(100);
    SDK_FREE(m100);
    return 0;
}