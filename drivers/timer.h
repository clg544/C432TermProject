/* The timers are 32 bit upward counters.
 */
#ifndef __TIMER__
#define __TIMER__

/* Base addresses section 2.1 */
#define DMTIMER0 0x44E05000

/* Offsets section 20.1.5 */
#define TCLR 0x38 /* Tiemr Control Register */
#define TCRR 0x3C /* Timer Counter Register */
#define TLDR 0x40 /* Timer Load Register */
#define TTGR 0x44 /* Timer Trigger Register */

/* TODO move some of these to macros? */

void TIMER_init(int timer, unsigned int value, int autoload);

void TIMER_set_counter(int timer, unsigned int value);

void TIMER_auto_reload(int timer, int autoload);

void TIMER_start(int timer);

void TIMER_stop(int timer);

int TIMER_finished(int timer);

void TIMER_delay(int timer, int value);

#endif
