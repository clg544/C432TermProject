/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <uart.h>

void printUIntHex(unsigned int character){
    char string[4];
    for (int i = 3; i >= 0; i--){
	string[3 - i] =  (character >> (i * 8));
    }
    UART_puts(UART_PORT0, &string, 4);
}