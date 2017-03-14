#include "common.h"
#include "timer.h"
#include "clock.h"

void TIMER_init(int timer, unsigned int value, int autoload) {
    CLOCK_enable_timer_clock(timer);
    TIMER_set_counter(timer, value);
    TIMER_auto_reload(timer, autoload);
}

/* The timer counts upward, which means you have to
 * set the timer to a large number for a short period.
 * This function makes it more intuitive by making a
 * small value for the counter take less time.
 */
void TIMER_set_counter(int timer, unsigned int value) {
    switch(timer) {
        case 0:
            DEREF(DMTIMER0 + TCRR) = value;
            break;
        default:
            break;
    }
}

void TIMER_auto_reload(int timer, int autoload) {
    switch(timer) {
        case 0:
            if (autoload)
                DEREF(DMTIMER0 + TCLR) |= 0x2; /* AR bit 1 */
            else
                DEREF(DMTIMER0 + TCLR) &= ~0x2; /* AR bit 1 */
            break;
        default:
            break;
    }

}

void TIMER_start(int timer) {
    switch(timer) {
        case 0:
            DEREF(DMTIMER0 + TCLR) |= 0x1; /* ST bit 0 */
            break;
        default:
            break;
    }
}

void TIMER_stop(int timer) {
    switch(timer) {
        case 0:
            DEREF(DMTIMER0 + TCLR) &= ~0x1; /* ST bit 0 */
            break;
        default:
            break;
    }
}


/* Timer stays at zero after overflow, assuming auto reload
 * is not enabled
 * */
int TIMER_finished(int timer) {
    switch(timer) {
        case 0:
            return (DEREF(DMTIMER0 + TCLR) & 0x1) == 0;
            break;
        default:
            break;
    }
    return -1;
}

void TIMER_delay(int timer, int value) {
    TIMER_set_counter(0, value);
    TIMER_start(0);
    while(!TIMER_finished(0)){}
}
