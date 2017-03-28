#include <common.h>
#include <interrupt.h>
#include "intc_hw.h"


extern void rtc_irq();
void und_entry() __attribute__((interrupt("UNDEF")));
void irq_entry() __attribute__((interrupt("IRQ")));
void fiq_entry() __attribute__((interrupt("FIQ")));

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

    /* load interrupt sub routines to proper location in RAM */
    DEREF(VECT_UND) = und_entry;
    DEREF(VECT_IRQ) = irq_entry;
    DEREF(VECT_FIQ) = fiq_entry;
}

/* undefined instruction */
void und_entry() {
    /* empty */
}

/* undefined instruction */
void fiq_entry() {
    /* empty */
}

void irq_entry() {
    switch(DEREF(INT_CONTROLLER+INTC_SIR_IRQ)) {
        case 75: // RTC timer
            rtc_irq();
            break;
        default:
            break;
    }
    DEREF(INT_CONTROLLER+INTC_CONTROL) = 1; // clear interrupt
}

