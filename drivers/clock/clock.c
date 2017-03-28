#include <clock.h>
#include <common.h>

void CLOCK_enable_gpio_clock(unsigned int port) {
    switch(port) {
        case 1:
            DEREF(CKM_PER + CM_PER_GPIO1_CLKCTRL) |= 0x2;
            break;
        default:
            break;
    }
}

void CLOCK_enable_timer_clock(unsigned int timer) {
    switch(timer) {
        case 0:
            DEREF(CM_WKUP + CM_WKUP_TIMER0_CLKCTRL) |= 0x2;
            break;
        case 2:
            DEREF(CKM_PER + CM_PER_TIMER2_CLKCTRL) |= 0x2;
            break;
        default:
            break;
    }
}
