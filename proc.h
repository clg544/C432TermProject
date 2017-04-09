#include "param.h"

#ifndef PROC_H
#define PROC_H 

enum procstate { SLEEPING, RUNNING, RUNNABLE, EXITED, UNUSED };
enum schedulePriority { HIGH, MEDIUM, LOW };

/* OS structure that records process information. */
struct process {
  enum procstate state;
  unsigned int pid;
  unsigned int parentPid;
  unsigned int *stack;   
  unsigned int *heap;
  enum schedulePriority priority;
};

/* Process Table that holds all processes */
size_t current_task;
struct process ptable[TASK_LIMIT];
unsigned int stacks[TASK_LIMIT][STACK_SIZE]; 

#endif /* PROC_H */

