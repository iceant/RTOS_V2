#ifndef INCLUDED_COMPILER_H
#define INCLUDED_COMPILER_H

#ifndef C_ASM
#   define C_ASM asm
#endif

#ifndef C_STATIC_FORCE_INLINE
#   define C_STATIC_FORCE_INLINE static inline __attribute__((always_inline))
#endif

#ifndef C_WEAK
#   define C_WEAK __attribute__((weak))
#endif

#ifndef C_ALIGNED
#   define C_ALIGNED(x) __attribute__((aligned(x)))
#endif

#ifndef C_PACKED
#   define C_PACKED __attribute__((packed))
#endif

#ifndef C_CONSTRUCTOR
#   define C_CONSTRUCTOR __attribute__((constructor))
#endif

#ifndef C_SECTION
#   define C_SECTION(n) __attribute__((section(#n)))
#endif



#endif /* INCLUDED_COMPILER_H */
