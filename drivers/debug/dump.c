/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <uart.h>
#include <debug.h>

const char* registers[10]= {
    "r0: ","r1: ","r2: ","r3: ","r4: ","r5: ","r6: ","r7: ","lr: ","sp: "
};

void dumpFrame(unsigned int * frame){
    for (int i = 0; i < 10; i++){
	UART_puts(UART_PORT0,(char *)registers[i],5);
	printUIntHex(frame[i]);
	terminateLine();
    }
}