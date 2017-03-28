/* The clock section deals with clock gating to MCU components.
 * It does not deal with handling timers them selves.
 */
#ifndef __CLOCK__
#define __CLOCK__

/* Base addresses section 2.1 */
#define CKM_PER 0x44E00000 /* clock module peripheral registers */
#define CM_WKUP 0x44E00400 /* clock mosule wakeup registers */
#define CONTROL_MODULE 0x44E10000


/* Offsets section 8.1.12 */
#define CM_PER_GPIO1_CLKCTRL 0x0AC
#define CM_PER_TIMER2_CLKCTRL 0x80
#define CM_WKUP_TIMER0_CLKCTRL 0x10

#define conf_uart0_rxd 0x970
#define conf_uart0_txd 0x974

void CLOCK_enable_gpio_clock(unsigned int port);

void CLOCK_enable_timer_clock(unsigned int timer);

#endif
