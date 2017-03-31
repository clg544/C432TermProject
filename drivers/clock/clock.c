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
            /* Thanks to TI starterware */
            DEREF(CKM_PER+CM_PER_L3S_CLKSTCTRL) |= 0x2; /* wake up */
            DEREF(CKM_PER+CM_PER_L3_CLKSTCTRL) |= 0x2; /* wake up */
            DEREF(CKM_PER+CM_PER_L3_INSTR_CLKCTRL) |= 0x2; /* Enable */
            DEREF(CKM_PER+CM_PER_OCWP_L3_CLKSTCTRL) |= 0x2; /* Enable */
            DEREF(CKM_PER+CM_PER_L4LS_CLKSTCTRL) |= 0x2; /* Enable */
            DEREF(CKM_PER+CM_PER_L4LS_CLKCTRL) |= 0x2; /* Enable */
            DEREF(CM_DPLL+CM_DPL_CLKSEL_TIMER2_CLK) |= 0x1; /* CLK M OSC */

            DEREF(CKM_PER + CM_PER_TIMER2_CLKCTRL) |= 0x2;
            break;
        default:
            break;
    }
}
