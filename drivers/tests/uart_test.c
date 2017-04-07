/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <interrupt_handler.h>
#include <uart.h>
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
    UART_puts(UART_PORT0, test,4);
    while(1){
	UARTCharPut(UART_PORT0, UARTCharGet(UART_PORT0));
    }
    return 0;
}