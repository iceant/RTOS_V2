#ifndef INCLUDED_SDK_MACROS_H
#define INCLUDED_SDK_MACROS_H

#define SDK_STRINGIFY(x) SDK_STRINGIFY2(x)
#define SDK_STRINGIFY2(x) #x
#define SDK_CAT(a, b) SDK_CAT2(a, b)
#define SDK_CAT2(a, b) a##b


#define SDK_MIN(a, b) (((a)>(b))?(b):(a))
#define SDK_MAX(a, b) (((a)<(b))?(b):(a))

#endif /* INCLUDED_SDK_MACROS_H */
