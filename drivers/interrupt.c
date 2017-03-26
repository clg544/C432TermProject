#include "common.h"
#include "interrupt.h"


extern void rtc_irq();
void und_isr() __attribute__((interrupt("UNDEF")));
void irq_isr() __attribute__((interrupt("IRQ")));

void unmask_irq(int int_num) {
    if(int_num >= 32 && int_num < 64) {
        DEREF(INT_CONTROLLER+INTC_MIR_CLEAR1) = 1<<(int_num-32);
    } else if(int_num >= 64) {
        DEREF(INT_CONTROLLER+INTC_MIR_CLEAR2) = 1<<(int_num-64);
    }
}

void irq_init() {
    DEREF(INT_CONTROLLER+INTC_SYSCONFIG) = 0x2; /* reset interrupt controller */
    while((DEREF(INT_CONTROLLER+INTC_SYSSTATUS)&0x1) == 0){} /* wait for reset complete */
    unmask_irq(75); /* unmask RTC interrupt */

    /* load interrupt sub routines to proper location in RAM */
    DEREF(VECT_UND) = und_isr;
    DEREF(VECT_IRQ) = irq_isr;
}

/* undefined instruction */
void und_isr() {
    /* empty */
}

void irq_isr() {
    switch(DEREF(INT_CONTROLLER+INTC_SIR_IRQ)) {
        case 75: // RTC timer
            rtc_irq();
            break;
        default:
            break;
    }
    DEREF(INT_CONTROLLER+INTC_CONTROL) = 1; // clear interrupt
}

