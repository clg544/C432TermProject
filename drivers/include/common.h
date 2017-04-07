#ifndef __COMMON__
#define __COMMON__

/* Macro for dereferencing pointers for memory mapped I/O */
#define DEREF(X) *(volatile unsigned int*)(X)
#define SET_BIT(p,n) ((p) |= ((1) << (n)))
#define CLR_BIT(p,n) ((p) &= ~((1) << (n)))
#define DEREF8(X) *(volatile char*)(X)

#endif
