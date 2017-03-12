#include "gpio.h"
#include "clock.h"
#include "common.h"


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
void GPIO_set_direction(int port, int dir) {
    switch(port) {
        case 1:
            DEREF(GPIO_PORT1 + GPIO_OE) &= ~(dir);
            break;
        default:
            break;
    }
}


void GPIO_write_port(int port, int value) {
    switch(port) {
        case 1:
            DEREF(GPIO_PORT1 + GPIO_DATAOUT) = value;
            break;
        default:
            break;
    }
}

int GPIO_read_port(int port) {
    return -1;
}
