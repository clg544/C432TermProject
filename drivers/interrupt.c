#include "common.h"

/* base address section 2.1*/
#define INT_CONTROLLER    0x48200000

#define VECT_UND 0x4030CE24 /* undefined instruction RAM exception vector */
#define VECT_IRQ 0x4030CE38 /* IRQ RAM exception vector */

/* offsets section 6.5.1 */
#define INTC_SYSCONFIG  0x10
#define INTC_SYSSTATUS  0x14
#define INTC_SIR_IRQ    0x40
#define INTC_CONTROL    0x48
#define INTC_MIR_CLEAR1 0xA8 /* masked interrupt clear 32-63 */
#define INTC_MIR_CLEAR2 0xC8 /* masked interrupt clear 64-95 */


extern void rtc_irq();
extern void irq_init_asm();
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

