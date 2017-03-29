/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
int volatile irq_count = 0;

void rtc_irq(){
    if (irq_count % 2){
	led_on(1);
    } else {
	led_off(1);
    }
    irq_count++;
}

extern void rtc_init();
int main() {
    /* int i; */
    led_init();
    rtc_init();
    irq_init();
    while(1){
    }
    return 0;
}
