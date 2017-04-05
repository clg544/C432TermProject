/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
#include <timer.h>
#include <common.h>


int main() {
    led_init();
    irq_init();
    TIMER_init(2, 100, 1, 1);
    TIMER_start(2);
    while(1){
    }
    return 0;
}
