/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
#include <uart.h>
#include <debug.h>
#include <timer.h>
char test[5] = "test";

extern void rtc_init();

int main() {
    /* int i; */
    led_init();
    irq_init();
    TIMER_init(2, 100, 1, 1);
    TIMER_start(2);
    UART_Init(0);
    asm volatile("eor r0,r0,r0");
    unsigned int* frame = captureFrame();
    dumpFrame(frame);
    return 0;
}