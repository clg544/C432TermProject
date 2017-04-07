#include <common.h>
#include <interrupt_handler.h>
#include <debug.h>
#include <led.h>
#include <timer.h>

//char[50] prfAbort = "Pre fetch abort";
//char dataAbort = "Data abort";
//char undAbort = "Undefined abort";
//char swiEntry = "Supervisor Instruction";

void clear_mir_line(int i){
    unsigned int reg = (i + 1) / 32;
    unsigned int bit = ((i) % 32);
    unsigned int mask = (1 << bit);
    DEREF(INT_CONTROLLER + INTC_MIR_CLEAR(reg)) = mask;
}

void set_mir_line(int i){
    unsigned int reg = (i + 1) / 32;
    unsigned int bit = ((i) % 32);
    unsigned int mask = (1 << bit);
    DEREF(INT_CONTROLLER + INTC_MIR_SET(reg)) = mask;
}


void irq_init() {
    DEREF(INT_CONTROLLER+INTC_SYSCONFIG) = 0x2; /* reset interrupt controller */
    while((DEREF(INT_CONTROLLER+INTC_SYSSTATUS)&0x1) == 0){} /* wait for reset complete */
    clear_mir_line(75); /* unmask RTC interrupt */
    clear_mir_line(68); /* unmask RTC interrupt */

    /* load interrupt sub routines to proper location in RAM */
    DEREF(VECT_UND) = und_entry;
    DEREF(VECT_SWI) = swi_entry;
    DEREF(VECT_PRF) = prf_entry;
    DEREF(VECT_DAB) = dab_entry;
    DEREF(VECT_IRQ) = irq_entry;
    DEREF(VECT_FIQ) = fiq_entry;
}
/* system call */
void swi_entry(){
    panic();
}
/* pre fetch abort */
void prf_entry(){
    panic();
}
/* data abort */
void dab_entry(){
    panic();
}
/* undefined instruction */
void und_entry() {
    panic();
}

/* undefined instruction */
void fiq_entry() {
    panic();
}

void irq_entry() {
    switch(DEREF(INT_CONTROLLER+INTC_SIR_IRQ)) {
        case 75: // RTC timer
            rtc_irq();
            break;
        case 68:
            dm_irq();
            DEREF(DMTIMER2+0x28) |= 0x2; // clear int
	    break;
        default:
            break;
    }
    DEREF(INT_CONTROLLER+INTC_CONTROL) = 1; // clear interrupt
}



