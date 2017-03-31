/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
#include <timer.h>
#include <common.h>
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
    irq_init();
    TIMER_init(0, 1000, 1, 1);
    led_on(2);
    TIMER_start(0);
    /* DEREF(DMTIMER0+TCLR) |= 0x2; */
    /* DEREF(DMTIMER0+TCLR) |= 0x1; */
    /* rtc_init(); */
    while(1){
    }
    return 0;
}
