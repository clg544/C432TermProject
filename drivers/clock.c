#include "clock.h"

void CLOCK_enable_gpio_clock(unsigned int port) {
    switch(port) {
        case 1:
            *(volatile unsigned int *)(CKM_PER + CM_PER_GPIO1_CLKCTRL) |= 0x2;
            break;
        default:
            break;
    }
}
