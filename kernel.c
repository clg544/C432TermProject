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

void startProc(struct process* proc){
	proc->state = RUNNING;

	bwputs("Entering process...\n");
	proc->stack = activate(proc->stack);	

	return;
}

int main(void) {
	/* I'm moving this into the proc struct
	unsigned int stacks[TASK_LIMIT][STACK_SIZE]; */
	/* Process Table that holds all processes */
	struct process *ptable[TASK_LIMIT];
	
	size_t task_count = 0;
	size_t current_task = 0;

	/* An initializing stack 
	unsigned int stacks[STACK_SIZE];	 */

	unsigned int s1[256];
	unsigned int s2[256];

	*(PIC + VIC_INTENABLE) = PIC_TIMER01;

	*TIMER0 = 15000;
	*(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_PERIODIC | TIMER_32BIT | TIMER_INTEN;

	/* 20-03-17 - Make an initial process */

	bwputs("Starting...\n");

	struct process p0;
	ptable[0] = &p0;
	ptable[0]->state = RUNNABLE;
	ptable[0]->pid = 0;
	ptable[0]->parentPid = 0;
	ptable[0]->stack = s1;	

	ptable[0]->stack = init_task(ptable[0]->stack, &first);
	task_count++;
	
	struct process p1;
	ptable[1] = &p1;
	ptable[1]->state = RUNNABLE;
	ptable[1]->pid = 1;
	ptable[1]->parentPid = 0;
	ptable[1]->stack = s2;	

	ptable[1]->stack = init_task(ptable[1]->stack, &second);
	task_count++;
	/* end */
	
	while(1) {
		ptable[current_task]->stack = activate(ptable[current_task]->stack);

		switch(ptable[current_task]->stack[2+7]) {
			
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


