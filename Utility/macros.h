#ifndef _MACROS_H_
#define _MACROS_H_

// The following are GNU C extensions that aid in code optimization, 
// specifically with respect to branch prediction.

#if defined __GNUC__

#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)

#else

#define likely(x) (x)
#define unlikely(x) (x)

#endif


#endif
