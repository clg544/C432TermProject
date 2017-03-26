#include "common.h"

/* base address section 2.1*/
#define INT_CONTROLLER    0x48200000

/* offsets section 6.5.1 */
#define INTC_SIR_IRQ      0x40
#define INTC_CONTROL      0x48

extern void rtc_irq();

/* undefined instruction */
void und_isr() __attribute__((interrupt("UNDEF")));
void und_isr() {
    /* empty */
}

void irq_isr() __attribute__((interrupt("IRQ")));
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

