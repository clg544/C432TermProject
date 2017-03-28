/* The timers are 32 bit upward counters.
 */
#ifndef __TIMER__
#define __TIMER__

/* Base addresses section 2.1 */
#define DMTIMER0     0x44E05000
#define DMTIMER1_1MS 0x44E31000

#define MS_TCLR 0x24
#define MS_CRR  0x28

typedef struct DMTIMER DMTIMER;

/* TODO move some of these to macros? */

void TIMER_init(DMTIMER* timer, unsigned int value, int autoload);

void TIMER_set_counter(DMTIMER* timer, unsigned int value);

void TIMER_set_auto_reload(DMTIMER* timer, int autoload);

void TIMER_start(DMTIMER* timer);

void TIMER_stop(DMTIMER* timer);

int TIMER_finished(DMTIMER* timer);

void TIMER_delay(DMTIMER* timer, int value);

#endif
