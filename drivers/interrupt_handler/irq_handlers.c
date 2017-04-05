/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>

int volatile rtc_count = 0;
int volatile dm_count = 0;

void rtc_irq(){
    if (rtc_count % 2){
	led_on(1);
    } else {
	led_off(1);
    }
    rtc_count++;
}

void dm_irq(){
    if (dm_count % 2){
	led_on(2);
    } else {
	led_off(2);
    }
    dm_count++;
}