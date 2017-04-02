/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
#include <timer.h>
#include <common.h>
#include <dmtimer.h>
#include <soc_AM335x.h>
#include <beaglebone.h>
#include <interrupt.h>

#define TIMER_INITIAL_COUNT (0xFF000000u)
#define TIMER_RLD_COUNT (0xFF000000u)
/* int volatile irq_count = 0; */

/* void rtc_irq(){ */
/*     if (irq_count % 2){ */
/* 	led_on(1); */
/*     } else { */
/* 	led_off(1); */
/*     } */
/*     irq_count++; */
/* } */

extern void rtc_init();
int main() {
    /* int i; */
    led_init();
    /* irq_init(); */
    DMTimer2ModuleClkConfig();
    IntMasterIRQEnable();
    IntAINTCInit();
    IntRegister(SYS_INT_TINT2, rtc_irq);
    IntPrioritySet(SYS_INT_TINT2, 0, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_TINT2);
    DMTimerCounterSet(SOC_DMTIMER_2_REGS, TIMER_INITIAL_COUNT);
    DMTimerReloadSet(SOC_DMTIMER_2_REGS, TIMER_RLD_COUNT);
    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
    DMTimerEnable(SOC_DMTIMER_2_REGS);
    /* rtc_init(); */
    while(1){
    }
    return 0;
}
