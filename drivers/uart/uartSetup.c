/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <common.h>
#include <uart.h>
#include <cm.h>
#include "uart_hw.h"

#define UART_CONSOLE_BASE                    (SOC_UART_0_REGS)
#define BAUD_RATE_115200                     (115200)
#define UART_MODULE_INPUT_CLK                (48000000)

void UART_Init(unsigned int port)
{
    switch(port){
	case 0:
	    /* Configuring the system clocks for UART0 instance. */
	    UART0ModuleClkConfig();

	    /* Performing the Pin Multiplexing for UART0 instance. */
	    UARTPinMuxSetup(0);

	    UARTStdioInitExpClk(BAUD_RATE_115200, 1, 1);
	    break;
	default:
	    break;
    }
}

void UARTModuleReset(unsigned int baseAdd)
{
    /* Performing Software Reset of the module. */
    DEREF(baseAdd + UART_SYSC) |= (UART_SYSC_SOFTRESET);

    /* Wait until the process of Module Reset is complete. */
    while(!(DEREF(baseAdd + UART_SYSS) & UART_SYSS_RESETDONE));
}