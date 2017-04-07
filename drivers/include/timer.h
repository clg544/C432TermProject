/* The timers are 32 bit upward counters.
 */
#ifndef __TIMER__
#define __TIMER__

/* Base addresses section 2.1 */
#define DMTIMER0     0x44E05000
#define DMTIMER1_1MS 0x44E31000
#define DMTIMER2     0x48040000
#define INTC         0x48200000

/* Offsets section 20.1.5 */
#define TCLR 0x38 /* Tiemr Control Register */
#define TCRR 0x3C /* Timer Counter Register */
#define TLDR 0x40 /* Timer Load Register */
#define TTGR 0x44 /* Timer Trigger Register */
#define TWPS 0x48 /* Timer write posting bits reg */
#define TSICR 0x54 /* Timer Synchronous Interface Control reg */
#define IRQENABLE_SET 0x2C /* Timer Interrupt enable set register*/

/* Write post status */
#define W_PEND_TCLR 0x0
#define W_PEND_TCRR 0x1
#define W_PEND_TLDR 0x2
#define W_PEND_TTGR 0x3
#define W_PEND_TMAR 0x4


#define MS_TCLR 0x24
#define MS_CRR  0x28

#define INTC_MIR_CLEAR2 0xC8

/* TODO move some of these to macros? */

void TIMER_init(int timer, unsigned int value, int autoload, int irq_enable);

void TIMER_set_counter(int timer, unsigned int value);

void TIMER_set_counter_ms(int timer, unsigned int milliseconds);

void TIMER_auto_reload(int timer, int autoload);

void TIMER_start(int timer);

void TIMER_stop(int timer);

int TIMER_finished(int timer);

void TIMER_delay(int timer, unsigned int value);

#endif
