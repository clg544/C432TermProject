#include "gpio.h"
/* #include "timer.h" */


int main() {
    int i;

    /* TIMER_init(0, 0xFFFF0000, 0); */
    GPIO_init_port(1);
    GPIO_set_direction(1, 1<<21);
    GPIO_write_port(1, 1<<27);
    /* for(i=0;i<1000;i++){} */
    /* GPIO_write_port(1, 0); */
    /* for(i=0;i<1000;i++){} */
    while(1) {
        /* GPIO_write_port(1, 1<<27); */
        /* TIMER_start(0); */
        /* while(!TIMER_finished(0)){} */
        /* for(i=0;i<1000;i++){} */
        /* GPIO_write_port(1, 0); */
        /* TIMER_set_counter(0, 0xFFFF0000); */
        /* TIMER_start(0); */
        /* for(i=0;i<10000;i++){} */
        /* while(!TIMER_finished(0)){} */
    }

    return 0;
}
