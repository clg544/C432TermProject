#include <stddef.h>

#include "versatilepb.h"
#include "asm.h"
#include "param.h"
#include "proc.h"

unsigned int *init_task(unsigned int* stack, void (*start)(void)) {
    stack += STACK_SIZE - 16; /* End of stack, minus what we're about to push */
    stack[0] = 0x10; /* User mode, interrupts on */
    stack[1] = (unsigned int)start;
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
    while(1) {
        bwputs("In first...\n");
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
 * int scheduler() - Choose the next process to run, return its' pid
 * Want to track what the previous process was? May or may not be useful
 *  - pid instead of index?
 * Start from index 1 of the ptable and loops over the ptable looking for the
 * next runnable process. Can't be negative
 */
int scheduler() {
    int next_task = 0;
    do{ 
        next_task++;
        if(next_task >= TASK_LIMIT){
            next_task = 0;
        }
    }while(ptable[next_task].state != RUNNABLE);
    /* Possible bug here if pids are being manually changed in main to match
     * the index of the process in the ptable
     */
    return ptable[next_task].pid;
}


int main(void) {
    int i;  
    size_t p;

    size_t task_count = 0;
    size_t current_task = 0;
    
    *(PIC + VIC_INTENABLE) = PIC_TIMER01;

    *TIMER0 = 15000;
    *(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_PERIODIC | TIMER_32BIT | TIMER_INTEN;

    for (i = 0; i < TASK_LIMIT; i++){
        ptable[i].state = UNUSED;
        ptable[i].stack = stacks[i];
    }

    bwputs("Starting...\n");

    /* Start an init process which will fork all other processes */
    ptable[task_count].stack = init_task(ptable[task_count].stack, &init);
    task_count++;
    
    while(1) {
        ptable[current_task].stack = startProc(&(ptable[current_task]));

        switch(ptable[current_task].stack[2+7]) {
            case 0x5: /* wait_pid */
                /* This implementation waits for *any* child to exit. */
                /* TODO: take pid as arg, and exit() will only wake this 
		 * task up if the corresponding pid exits. */
                /* The return value is set by the exit() call. 
		 * The exit() call takes care of waking this parent up. */
                ptable[current_task].state = SLEEPING;
                break;
            case 0x4: /* end */
                task_count--;
                ptable[current_task].state = EXITED;
                /* Return this tasks exit-code to parent. */
                /* TODO: Should return the argument to exit (which is the 
		 * tasks exit-code). Default 0 for now. */
                ptable[ptable[current_task].parentPid].stack[2+0] = 0;
                /* Wake up parent if it is sleeping because of wait_pid 
		 * (currently the only way to sleep) */
                /* TODO: Only wake it up if it is waiting for *this* pid to exit. */
                if(ptable[ptable[current_task].parentPid].state == SLEEPING){ 
		    /* && parent.wait_pid == current_task */
                    ptable[ptable[current_task].parentPid].state = RUNNABLE;
                }
                /* Re-assign orphaned children to their grandparent. */
                for(p = 0; p < TASK_LIMIT; p++){
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
                    /* Find a free proc to store new process: as long as 
		     * task_count < TASK_LIMIT this should find a free proc. */
                    for(p = 0; p < TASK_LIMIT; p++){
                        if(ptable[p].state == UNUSED || ptable[p].state == EXITED){
                            break;
                        }
                    }
                    /* We were somehow unable to find a free proc to use. This case 
		     * likely indicates an implementation error in one of our syscalls. */
                    if(p == TASK_LIMIT){
                        bwputs("Fork Failed.\n"); 
                        /* Cannot create a new task, return error */
                        ptable[current_task].stack[2+0] = -1;
                        break;
                    }
                    /* p now points to a free proc struct ready to be used. */
                    /* New stack is END - used */
                    ptable[p].stack = ptable[p].stack + STACK_SIZE - used;
                    /* Copy only the used part of the stack */
                    memcpy(ptable[p].stack, ptable[current_task].stack,
                           used*sizeof(ptable[current_task].stack[0]));
                    /* Copy process values into the new proc */
                    ptable[p].state = RUNNABLE;
                    ptable[p].pid = p; /* this makes a processe's pid match its 
		                        * index into the ptable. */
                    ptable[p].parentPid = ptable[current_task].pid; 
                    
                    /* Set return values in each process */
                    ptable[current_task].stack[2+0] = p;
                    ptable[p].stack[3+0] = 0;
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

        /* This is technically our scheduler: skip over sleeping or exited 
	 * processes to find a RUNNABLE one. */
        /* this is also where scheduler() should run. It needs to return a 
	 * pid that it has selected to run */
        current_task = scheduler();
    }
    
    return 0;
}


