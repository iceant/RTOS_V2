#ifndef INCLUDED_SDK_MEMORY_H
#define INCLUDED_SDK_MEMORY_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/


////////////////////////////////////////////////////////////////////////////////
////

void* sdk_malloc(sdk_size_t bytes, const char* file, sdk_size_t line);
void* sdk_calloc(sdk_size_t count, sdk_size_t bytes, const char* file, sdk_size_t line);
void* sdk_realloc(void* mem, sdk_size_t bytes, const char* file, sdk_size_t line);
void  sdk_free(void* mem, const char* file, sdk_size_t line);

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ALLOC(x) sdk_malloc((x), __FILE__, __LINE__)
#define SDK_CALLOC(n, x) sdk_calloc((n), (x), __FILE__, __LINE__)
#define SDK_REALLOC(p, x) sdk_realloc((p), (x), __FILE__, __LINE__)
#define SDK_FREE(p) (sdk_free((p), __FILE__, __LINE__), (p) = 0)

#define SDK_NEW(p) (p) = SDK_ALLOC(sizeof(*(p)))
#define SDK_NEW0(p) (p) = SDK_CALLOC(1, sizeof(*(p)))
#define SDK_RESIZE(p, x) (p) = SDK_REALLOC(p, x)

#endif /* INCLUDED_SDK_MEMORY_H */
