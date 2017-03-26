#ifndef __INTERRUPT__
#define __INTERRUPT__

/* base address section 2.1*/
#define INT_CONTROLLER    0x48200000

#define VECT_UND 0x4030CE24 /* undefined instruction RAM exception vector */
#define VECT_IRQ 0x4030CE38 /* IRQ RAM exception vector */

/* offsets section 6.5.1 */
#define INTC_SYSCONFIG  0x10
#define INTC_SYSSTATUS  0x14
#define INTC_SIR_IRQ    0x40
#define INTC_CONTROL    0x48
#define INTC_MIR_CLEAR1 0xA8 /* masked interrupt clear 32-63 */
#define INTC_MIR_CLEAR2 0xC8 /* masked interrupt clear 64-95 */

void irq_init();


#endif
