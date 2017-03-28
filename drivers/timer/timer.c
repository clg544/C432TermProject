#include <common.h>
#include <timer.h>
#include <clock.h>
#include "dmtimer_hw.h"

void TIMER_init(DMTIMER* timer, unsigned int value, int autoload) {
    CLOCK_enable_timer_clock(0);
    TIMER_set_auto_reload(timer, autoload);
}

/* The timer counts upward, which means you have to
 * set the timer to a large number for a short period.
 * This function makes it more intuitive by making a
 * small value for the counter take less time.
 */
void TIMER_set_counter(unsigned int timer, unsigned int value) {
    DEREF(timer + DMTIMER_TCRR) = 0xFFFFFFFF - value;
}

void TIMER_set_counter_ms(unsigned int timer, unsigned int miliseconds) {
    unsigned int value = 31*miliseconds;
    DEREF(timer + DMTIMER_TCRR)  = 0xFFFFFFFF - value;
    /* DEREF(DMTIMER1_1MS + MS_TCRR) = 0xFFFFFFFF - value; */
}

void TIMER_set_auto_reload(unsigned timer, int value) {
    unsigned int mask = ~(1 << 1);
    DEREF(timer + DMTIMER_TCLR) = DEREF(timer + DMTIMER_TCLR) | ((~mask) & (value << 1));
}

void TIMER_start(unsigned int timer) {
    DEREF(timer + DMTIMER_TCLR) |= 0x1; /* ST bit 0 */  
}

void TIMER_stop(unsigned int timer) {
    DEREF(timer + DMTIMER_TCLR) &= ~0x1; /* ST bit 0 */
}

/* Timer stays at zero after overflow, assuming auto reload
 * is not enabled
 * */
int TIMER_finished(unsigned int timer) {
   volatile int i = ((DEREF(timer + DMTIMER_TCLR) & 0x1) == 0);
   return i;
}

void TIMER_delay(unsigned int timer, int value) {
    asm volatile ("dsb");
    TIMER_set_counter(timer, value);
    asm volatile ("dsb");
    TIMER_start(timer);
    asm volatile ("dsb");
    while(!TIMER_finished(timer));
    asm volatile ("dsb");
}
