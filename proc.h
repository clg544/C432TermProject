#include "param.h"

#ifndef PROC_H
#define PROC_H 

enum procstate { SLEEPING, RUNNING, RUNNABLE, KILLED };

/* Struct that holds a snapshot of the process. */
struct context { /*{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}*/
  int r0;
  int r1;
  int r2;
  int r3;
  int r4;
  int r5;
  int r6;
  int r7;
  int r8;
  int r9;
  int r10;
  int r11;
  int r12;
  int sp;
  int lr;
  int pc;
  int apsr;
} __attribute__ ((packed));


/* OS structure that records process information. */
struct process {
  enum procstate state;
  unsigned int pid;  
  
  int parentPid;
  unsigned int *stack;   
  struct context pcontext;
};

#endif /* PROC_H */

