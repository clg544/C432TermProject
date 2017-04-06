#include <stddef.h>

#include "versatilepb.h"
#include "asm.h"
#include "param.h"
#include "proc.h"
#include "malloc.h"

unsigned int *init_task(unsigned int* stack, void (*start)(void)) {
    stack += STACK_SIZE - 24; /* End of stack, minus what we're about to push */
    stack[0] = 0x10; /* User mode, interrupts on */
    stack[1] = (unsigned int)start;
    stack[2] = 0;
    return stack;
}

/**
 * void memcpy(void *dest, void *src, size_t n) - Copy n bytes of memory from
 *src to dest.
 */
void *memcpy(void *dest, void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    size_t i;
    for(i = 0; i < n; i++){
        d[i] = s[i];
    }
    return d;
}


/**
 * void bwputs(char *s) - Given a string, feed it to the serial port for display
 */
void bwputs(char* s){
    while(*s){
        while(*(UART0 + UARTFR) & UARTFR_TXFF);
        *UART0 = *s;
        s++;
    }
}


/**
 * void first(void) - Simple User program for testing user mode
 */
void first(void) {
    char *str;

    bwputs("Enter Malloc\n");
    str = (char*)memalloc((size_t)13);

    str[0] = 'H';


    while(1) {
        bwputs(str);
    }
}
void second(void) {
    while(1) {
        bwputs("In second...\n");
    }
}

/**
 * unsigned int *startProc() - Handle the process data while starting program.
 */
unsigned int *startProc(struct process *proc){
    proc->state = RUNNING;

    /* Activate the process in assembly */
    /* This only returns because an interrupt or syscall happened. */
    proc->stack = activate(proc->stack);    

    /* States do influence stuff now */
    proc->state = RUNNABLE;

    /* Return with the interrupt code on the stack */
    return proc->stack;
}


/**
 * void init() - The init process from which all other procs branch
 */
void init(){
    int r;

    r = fork();

    if (r < 0){
        /* Fork failed, exit forever*/
        return;
    }   
    else if(r == 0){
        /* This is our forked process, give it a task */
        first();
    }
    /* r contains the pid of the forked child. Continue... */

    r = fork();

    if (r < 0){
        /* Fork failed, exit forever*/
        return;
    }   
    else if(r == 0){
        /* This is our forked process, give it a task */
        second();
    }

    while(1){}; 
    /* exit init */
    return;
}

/*
int free(void* mem, size_t current_task){
    size_t *;

    if(mem < ptable[current_task].stack || mem > ptable[current_task].heap){
	return 0;
    }
    
    i = ptable[current_task].heap;
    while(mem > i){
         
    

}*/

/* Add another allocation to the heap.
 *
 * The heap is organized with a header that malloc will iterate through until
 * it fist a suitable chunk of memory for the program.
 *
 * [ isFree ]
 * [ block size ]
 * [ next header]
 * [ memory]
 * [ ... ]
 * [ endMemory ]
 */
void *allocateHeap(size_t size){
    size_t *i, *next;
    size_t header_size = 3;
    
    if(size <= 0) return 0;

    if (ptable[current_task].heap - header_size - size < 
		    ptable[current_task].stack){
        /* Not enough room */ 
        return 0;
    }

    /* Set to first free space variable of the heap */
    i = ptable[current_task].heap;
    next = 0;
    while (size+header_size < (unsigned int)ptable[current_task].heap - 
                     (unsigned int)ptable[current_task].stack || !((i-2) == 0)){
        /* If the heap is free, and our data will fit, */
        if(*i == 1 && *(i - 1) > size){
            *i = 0;
            *(i-1) = size;
	    next = i - header_size - size;
            *(i-2) = (size_t) next;

	    if(*(i-2) == 0){
		/* Point to new node */
		*(i-2) = (size_t)next;

                *next = 1;
		*(next-1) = *(i-2) - (unsigned int)ptable[current_task].stack;
		*(next-2) = 0;
            }
	    else if (*(i-2) != 0){
                *next = 1;
		*(next-1) = *(i-1) - size;
		*(next-2) = *(i-2);
                
		*(i) = (size_t)0;
		*(i-1) = size;
		*(i-2) = (size_t)next;
	    }

	    return  (i - header_size - size) + 1; 
	}
	else{
	    i = next;
	}
    }
    
    /* Search failed */
    return 0;
}


int main(void) {
    int i;  
    size_t p;

    size_t task_count = 0;
    current_task = 0;
    
    *(PIC + VIC_INTENABLE) = PIC_TIMER01;

    *TIMER0 = 15000;
    *(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_PERIODIC | TIMER_32BIT | TIMER_INTEN;

    for (i = 0; i < TASK_LIMIT; i++){
	ptable[i].state = UNUSED;
        ptable[i].stack = stacks[i];
	ptable[i].heap = stacks[i] + STACK_SIZE - 1;
	ptable[i].stack[STACK_SIZE-1] = 1; /* heap node is free */
	ptable[i].stack[STACK_SIZE-2] = ptable[i].heap - ptable[i].stack; /* Heap size */
	ptable[i].stack[STACK_SIZE-3] = 0; /* There are no other heap headers */  
    }

    bwputs("Starting...\n");

    /* Start an init process which will fork all other processes */
    ptable[task_count].stack = init_task(ptable[task_count].stack, &init);
    task_count++;
    
    while(1) {
        ptable[current_task].stack = startProc(&(ptable[current_task]));

        switch(ptable[current_task].stack[2+7]) {
            case 0x7: /* memalloc */ 
		/* get the size parameter from the user stack */
		p = (size_t)(ptable[current_task].stack[2]);
		allocateHeap(p);
		
		break;
            case 0x5: /* wait_pid */
                /* This implementation waits for *any* child to exit. */
                /* TODO: take pid as arg, and exit() will only wake this task up if the corresponding pid exits. */
                /* The return value is set by the exit() call. The exit() call takes care of waking this parent up. */
                ptable[current_task].state = SLEEPING;
                break;
            case 0x4: /* end */
                task_count--;
                ptable[current_task].state = EXITED;
                /* Return this tasks exit-code to parent. */
                /* TODO: Should return the argument to exit (which is the tasks exit-code). Default 0 for now. */
                ptable[ptable[current_task].parentPid].stack[2+0] = 0;
                /* Wake up parent if it is sleeping because of wait_pid (currently the only way to sleep) */
                /* TODO: Only wake it up if it is waiting for *this* pid to exit. */
                if(ptable[ptable[current_task].parentPid].state == SLEEPING){ /* && parent.wait_pid == current_task */
                    ptable[ptable[current_task].parentPid].state = RUNNABLE;
                }
                /* Re-assign orphaned children to their grandparent. */
                for(p = 0; p < task_count; p++){
                    if(ptable[p].parentPid == current_task){
                        ptable[p].parentPid = ptable[current_task].parentPid;
                    }
                }
                break;
            case 0x3: /* get_parent_pid */
                ptable[current_task].stack[2+0] = ptable[current_task].parentPid;
                break;
            case 0x2: /* get_pid */
                   /* Could technically just be = current_task; */
                ptable[current_task].stack[2+0] = ptable[current_task].pid;
                break;
            case 0x1: /* fork */
                bwputs("fork...");
                if(task_count == TASK_LIMIT) {
                    bwputs("Fork Failed.\n");
                    /* Cannot create a new task, return error */
                    ptable[current_task].stack[2+0] = -1;
                } else {
                    bwputs("Fork Success\n");
                    /* Compute how much of the stack is used */
                    size_t used = (int) (ptable[current_task].stack + STACK_SIZE
                                  - ptable[current_task].stack);
                    /* New stack is END - used */
                    ptable[task_count].stack = ptable[task_count].stack + STACK_SIZE - used;
                    /* Copy only the used part of the stack */
                    memcpy(ptable[task_count].stack, ptable[current_task].stack,
                           used*sizeof(ptable[current_task].stack[0]));
                    
                    /* Copy process values into the new proc */
                    ptable[task_count].state = RUNNABLE;
                    ptable[task_count].pid = task_count;
                    ptable[task_count].parentPid = ptable[current_task].pid; 
                    
                    /* Set return values in each process */
                    ptable[current_task].stack[2+0] = task_count;
                    ptable[task_count].stack[2+0] = 0;
                    /* There is now one more task */
                    task_count++;
                }
                break;
            default: /* Catch all interrupts */
                if(*(TIMER0 + TIMER_MIS)) { /* Timer0 went off */
                    *(TIMER0 + TIMER_INTCLR) = 1; /* Clear interrupt */
                    bwputs("tick!\n");
                }
        }

        /* This is technically our scheduler: skip over sleeping or exited processes to find a RUNNABLE one. */
        /* this is also where scheduler() should run. It needs to return a pid that it has selected to run */
        do{ 
            current_task++;
            if(current_task >= task_count){
                current_task = 0;
            }
        } 
	while(ptable[current_task].state != RUNNABLE);
    }
    
    return 0;
}


