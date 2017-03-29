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
	proc->stack = activate(proc->stack);	

	/* States don't influence anything yet */
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
	else if(r > 0){
		/* This is our forked process, give it a task */
		first();
	}
	/* else, 0. Continue... */

	r = fork();

	if (r < 0){
		/* Fork failed, exit forever*/
		return;
	}	
	else if(r > 0){
		/* This is our forked process, give it a task */
		second();
	}

	while(1){};	
	/* exit init */
	return;
}



int main(void) {
	int i;	

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

	/* In the future, we will start an init process and fork all other code from there */
	ptable[task_count].stack = init_task(ptable[task_count].stack, &init);
	task_count++;
	
	while(1) {
		ptable[current_task].stack = startProc(&(ptable[current_task]));

		switch(ptable[current_task].stack[2+7]) {
			case 0x1: /* fork */
				bwputs("fork...");
				if(task_count == TASK_LIMIT) {
					bwputs("Fork Failed.");
					/* Cannot create a new task, return error */
					ptable[current_task].stack[2+0] = -1;
				} else {
					bwputs("Fork Success");
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
					ptable[task_count].stack[3+0] = 0;
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

		current_task++;
		if(current_task >= task_count) current_task = 0;
	}
	
	return 0;
}


