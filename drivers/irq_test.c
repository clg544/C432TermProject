#include "gpio.h"
#include "timer.h"
#include "common.h"
/* #include "interrupt.h" */

extern void irq_init();
extern void rtc_init();

volatile int irq_count;

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
