#ifndef __COMMON__
#define __COMMON__

/* Macro for dereferencing pointers for memory mapped I/O */
#define DEREF(X) *(volatile unsigned int*)(X)
#define DEREF8(X) *(volatile char*)(X)

#endif
