#include "common.h"
#include "timer.h"
#include "clock.h"
#include <led.h>

void wait_for_write(int reg) {
    if(DEREF(DMTIMER0+TSICR)&0x4) {
        while(DEREF(DMTIMER0+TWPS)&reg);
    }
}

void TIMER_init(int timer, unsigned int value, int autoload, int irq_enable) {
    CLOCK_enable_timer_clock(timer);
    TIMER_set_counter_ms(timer, value);
    TIMER_auto_reload(timer, autoload);
    /* if(1 == irq_enable) { */
        DEREF(DMTIMER2+IRQENABLE_SET) = 0x2;
        /* DEREF(INTC + INTC_MIR_CLEAR2) |= 1<<11; */
    /* } */
}

/* The timer counts upward, which means you have to
 * set the timer to a large number for a short period.
 * This function makes it more intuitive by making a
 * small value for the counter take less time.
 */
void TIMER_set_counter(int timer, unsigned int value) {
    switch(timer) {
        case 0:
            wait_for_write(W_PEND_TCRR);
            DEREF(DMTIMER0 + TCRR) = 0xFFFFFFFF - value;
            wait_for_write(W_PEND_TLDR);
            DEREF(DMTIMER0 + TLDR) = 0xFFFFFFFF - value;
            break;
        case 2:
            DEREF(DMTIMER2 + TCRR) = 0xFFFFFFFF - value;
            DEREF(DMTIMER2 + TLDR) = 0xFFFFFFFF - value;
            break;
        default:
            break;
    }
}

void TIMER_set_counter_ms(int timer, unsigned int miliseconds) {
    unsigned int value = 31*miliseconds;
    /* switch(timer) { */
    /*     case 0: */
            wait_for_write(W_PEND_TCRR);
            DEREF(DMTIMER0 + TCRR) = 0xFFFFFFFF - value;
            wait_for_write(W_PEND_TLDR);
            DEREF(DMTIMER0 + TLDR) = 0xFFFFFFFF - value;
    /*         break; */
    /*     case 2: */
    /*         DEREF(DMTIMER2 + TCRR) = 0xFFFFFFFF - value; */
    /*         break; */
    /*     default: */
    /*         break; */
    /* } */
}

void TIMER_auto_reload(int timer, int autoload) {
    switch(timer) {
        case 0:
            if (autoload) {
                wait_for_write(W_PEND_TCLR);
                DEREF(DMTIMER0 + TCLR) |= 0x2; /* AR bit 1 */
            }
            else {
                wait_for_write(W_PEND_TCLR);
                DEREF(DMTIMER0 + TCLR) &= ~0x2; /* AR bit 1 */
            }
            break;
        case 2:
            if (autoload)
                DEREF(DMTIMER2 + TCLR) |= 0x2; /* AR bit 1 */
            else
                DEREF(DMTIMER2 + TCLR) &= ~0x2; /* AR bit 1 */
        default:
            break;
    }

}

void TIMER_start(int timer) {
    switch(timer) {
        case 0:
            wait_for_write(W_PEND_TCLR);
            DEREF(DMTIMER0 + TCLR) |= 0x1; /* ST bit 0 */
            break;
        case 2:
            DEREF(DMTIMER2 + TCLR) |= 0x1; /* ST bit 0 */
            break;
        default:
            break;
    }
}

void TIMER_stop(int timer) {
    switch(timer) {
        case 0:
            wait_for_write(W_PEND_TCLR);
            DEREF(DMTIMER0 + TCLR) &= ~0x1; /* ST bit 0 */
            break;
        case 2:
            DEREF(DMTIMER2 + TCLR) &= ~0x1; /* ST bit 0 */
            break;
        default:
            break;
    }
}


/* Timer stays at zero after overflow, assuming auto reload
 * is not enabled
 * */
int TIMER_finished(int timer) {
    int val = -1;
    switch(timer) {
        case 0:
            return (DEREF(DMTIMER0 + TCLR) & 0x1) == 0;
            break;
        case 2:
            return (DEREF(DMTIMER2 + TCLR) & 0x1) == 0;
            break;
        default:
            break;
    }
    return val;
}

void TIMER_delay(int timer, unsigned int value) {
    TIMER_set_counter_ms(0, value);
    TIMER_start(timer);
    while(!TIMER_finished(timer)){}
}

