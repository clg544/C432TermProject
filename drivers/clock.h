#ifndef __CLOCK__
#define __CLOCK__

/* Base addresses section 2.1 */
#define CKM_PER 0x44E00000 /* clock module peripheral registers */


/* Offsets section 8.1.12 */
#define CM_PER_GPIO1_CLKCTRL 0x0AC

void CLOCK_enable_gpio_clock(unsigned int port);

#endif
