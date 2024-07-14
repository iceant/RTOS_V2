#include <sdk_memory.h>

#if defined __has_include
#  if __has_include (<tlsf.h>)
#    include <tlsf.h>
#  endif
#endif

#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
////
#if defined __has_include
#  if __has_include (<tlsf.h>)

static void* sdk_memory__tlsf_pool = 0;

static void sdk_memory_destroy(void){
    if(sdk_memory__tlsf_pool!=0){
        destroy_memory_pool(sdk_memory__tlsf_pool);
        printf("sdk_memory_destroy(%p)\n", sdk_memory__tlsf_pool);
        free(sdk_memory__tlsf_pool);
    }
}

static C_CONSTRUCTOR
size_t sdk_memory_init(void){
    sdk_memory__tlsf_pool = malloc(TLSF_POOL_SIZE);
    if(!sdk_memory__tlsf_pool){
        return -1;
    }
    size_t size =  init_memory_pool(TLSF_POOL_SIZE, sdk_memory__tlsf_pool);
    printf("sdk_memory_init(%u/%u)\n", size, TLSF_POOL_SIZE);
    atexit(sdk_memory_destroy);
    return size;
}

void* sdk_malloc(sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return tlsf_malloc(bytes);
}
void* sdk_calloc(sdk_size_t count, sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return tlsf_calloc(count, bytes);
}

void* sdk_realloc(void* mem, sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return tlsf_realloc(mem, bytes);
}

void sdk_free(void* mem, const char* file, sdk_size_t line)
{
    if(mem){
        tlsf_free(mem);
    }
}

#  else
void* sdk_malloc(sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return malloc(bytes);
}
void* sdk_calloc(sdk_size_t count, sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return calloc(count, bytes);
}

void* sdk_realloc(void* mem, sdk_size_t bytes, const char* file, sdk_size_t line)
{
    return realloc(mem, bytes);
}

void sdk_free(void* mem, const char* file, sdk_size_t line)
{
    if(mem){
        free(mem);
    }
}
#  endif

#endif


