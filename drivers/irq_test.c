#include "gpio.h"
#include "timer.h"
#include "common.h"
extern void und_isr();
extern void irq_isr();
extern void irq_init();
extern void rtc_init();
extern void rtc_irq();

volatile int irq_count;
#define SOC_AINTC_REGS    0x48200000  // BBB ARM Interrupt Controller base address
#define INTC_SIR_IRQ      0x40
#define INTC_CONTROL      0x48

void irq_isr() __attribute__((interrupt("IRQ")));
void irq_isr() {
    switch(DEREF(SOC_AINTC_REGS+INTC_SIR_IRQ)) {
        case 75: // RTC timer
            rtc_irq();
            break;
        default:
            break;
    }
    DEREF(SOC_AINTC_REGS+INTC_CONTROL) = 1; // clear interrupt
}


void rtc_irq() {
    irq_count++;
}

int main() {
    int old_count = 0;
    int j = 0;
    irq_count = 0;
    /* TIMER_init(0, 1000000, 0, 0); */
    GPIO_init_port(1);
    GPIO_set_direction(1, 3<<21|1<<23);
    rtc_init();
    irq_init();
    while(1) {
        if(old_count != irq_count) {  // trigger LED on changed irq_count
              old_count = irq_count;
              j = 0;
              GPIO_write_port(1, 1<<21);
              while (j < 200000) j++;
              GPIO_write_port(1, 0);
        }
    }

    return 0;
}
