#include "gpio.h"
#include "timer.h"
#include "common.h"

void IRQ_handler() __attribute__ ((interrupt ("IRQ")));
void IRQ_handler() {
    GPIO_write_port(1, 1<<21);
}

int main() {
    /* int i; */

    TIMER_init(2, 1000000, 0, 0);
    GPIO_init_port(1);
    GPIO_set_direction(1, 3<<21|1<<23);
    /* TIMER_delay(0, 1000); */
    /* GPIO_write_port(1, 1<<23); */
    /* TIMER_delay(0, 1000); */
    /* GPIO_write_port(1, ~(1<<23)); */
    /* DEREF(DMTIMER0+IRQENABLE_SET) = 0x2; */
    /* TIMER_set_counter_ms(0, 1000); */
    /* TIMER_start(0); */
    /* if((DEREF(DMTIMER0+IRQENABLE_SET)&0x02) == 0x02) */
    /*     GPIO_write_port(1, 1<<22); */
    while(1) {
        GPIO_write_port(1, 1<<21);
        TIMER_delay(2, 1000);
        GPIO_write_port(1, 0);
        TIMER_delay(2, 1000);
    }

    return 0;
}
