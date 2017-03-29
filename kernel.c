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
		fork();
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

}



int main(void) {
	int i;	

	/* Process Table that holds all processes */
	struct process ptable[TASK_LIMIT];
	unsigned int stacks[TASK_LIMIT][STACK_SIZE];	

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
	ptable[0].stack = init_task(ptable[0].stack, &first);
	task_count++;
	
	ptable[1].stack = init_task(ptable[1].stack, &second);
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
					              - &stacks[current_task][0]);
					/* New stack is END - used */
					ptable[task_count].stack = stacks[task_count] + STACK_SIZE - used;
					/* Copy only the used part of the stack */
					memcpy(stacks[task_count], stacks[current_task],
					       used*sizeof(stacks[current_task][0]));
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
					bwputs("\n\ntick!\n\n");
				}
		}

		current_task++;
		if(current_task >= task_count) current_task = 0;
	}
	
	return 0;
}


