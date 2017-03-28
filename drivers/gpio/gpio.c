#include <gpio.h>
#include "gpio_hw.h"
#include <clock.h>
#include <common.h>

void GPIO_init_port(int port) {
    switch(port) {
        case 1:
            CLOCK_enable_gpio_clock(1);
            break;
        case 2:
            //enable clock on p2
            break;
        default:
            break;
    }
}

/* TODO make this more clear, define INPUT and OUTPUT */
void GPIO_set_direction(unsigned int baseAdd, int dir) {
    DEREF(baseAdd + GPIO_OE) = dir;
}


void GPIO_write_port(unsigned int baseAdd, int port, int value) {
    unsigned int mask = ~(1 << port);
    DEREF(baseAdd + GPIO_DATAOUT) = ((DEREF(baseAdd + GPIO_DATAOUT) & mask) | (value << port));
}

int GPIO_read_port(unsigned int baseAdd) {
    return -1;
}
