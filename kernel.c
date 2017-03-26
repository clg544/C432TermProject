#include <stddef.h>

#include "versatilepb.h"
#include "asm.h"
#include "param.h"
#include "proc.h"

unsigned int *init_task(unsigned int *stack, void (*start)(void)) {
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
 * void task(void) - Second process for multitasking 
 */
void task(void){
	bwputs("In other task...\n");
	while(1) syscall();
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
 * The initial task from which all others are made.
 */
void init(void){

	while(1) {
		bwputs("In init...\n");
	}

}

struct process *startProc(struct process* proc){
	proc->state = RUNNING;

	bwputs("Entering process...\n");
	activate(proc->stack);

	return proc;
}

int main(void) {
	/* I'm moving this into the proc struct
	unsigned int stacks[TASK_LIMIT][STACK_SIZE]; */
	/* Process Table that holds all processes */
	struct process *ptable[TASK_LIMIT];
	/* The currently running proc */
	struct process *curProc;	

	size_t task_count = 0;
	size_t current_task = 0;

	/* An initializing stack */
	unsigned int stack[STACK_SIZE];
		

	*(PIC + VIC_INTENABLE) = PIC_TIMER01;

	*TIMER0 = 15000;
	*(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_PERIODIC | TIMER_32BIT | TIMER_INTEN;

	/* 20-03-17 - Make an initial process */

	bwputs("Starting...\n");
	
	ptable[0]->state = RUNNABLE;
	ptable[0]->pid = 0;
	ptable[0]->parentPid = 0;
	ptable[0]->stack = stack;	

	ptable[0]->stack = init_task(ptable[0]->stack, &init);
	task_count++;
	
	/* end */
	
	while(1) {
		curProc = startProc(ptable[current_task]);

		switch(curProc->stack[2+7]) {
			case 0x1:
				if(task_count == TASK_LIMIT) {
					/* Cannot create a new task, return error */
					curProc->stack[2+0] = -1;
				} else {
					/* Compute how much of the stack is used */
					size_t used = curProc->stack + STACK_SIZE
					              - curProc->stack;
					/* New stack is END - used */
					curProc->stack = ptable[task_count]->stack + STACK_SIZE - used;
					/* Copy only the used part of the stack */
					memcpy(ptable[task_count]->stack, curProc->stack,
					       used*sizeof(*ptable[current_task]));
					/* Set return values in each process */ 
					curProc->stack[2+0] = task_count;
					ptable[task_count]->stack[2+0] = 0; 
					/* There is now one more task */
					task_count++;
				}
				break;
			case -4: /* Timer 0 or 1 went off */
				if(*(TIMER0 + TIMER_MIS)) { /* Timer0 went off */
				*(TIMER0 + TIMER_INTCLR) = 1; /* Clear interrupt */
				     bwputs("tick\n");
				    } 
		}

		current_task++;
		if(current_task == task_count) current_task = 0;
	}
	
	return 0;
}

