#include "gpio.h"
#include "timer.h"


int main() {
    /* int i; */

    TIMER_init(0, 1000000, 0, 0);
    GPIO_init_port(1);
    GPIO_set_direction(1, 1<<21);
    while(1) {
        GPIO_write_port(1, 1<<21);
        TIMER_delay(0, 1000);
        GPIO_write_port(1, 0);
        TIMER_delay(0, 1000);
    }

    return 0;
}
