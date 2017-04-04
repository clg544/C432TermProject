#include <common.h>
#include <interrupt_handler.h>
/* #include "intc_hw.h" */
#include <led.h>
#include <timer.h>

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
    clear_mir_line(68); /* unmask RTC interrupt */

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
        case 68:
            rtc_irq();
            DEREF(DMTIMER2+0x28) |= 0x2; // clear int
        default:
            break;
    }
    DEREF(INT_CONTROLLER+INTC_CONTROL) = 1; // clear interrupt
}

int volatile irq_count = 0;

void rtc_irq(){
    if (irq_count % 2){
	led_on(1);
    } else {
	led_off(1);
    }
    irq_count++;
    DEREF(DMTIMER2+0x28) |= 0x2; // clear int
}

