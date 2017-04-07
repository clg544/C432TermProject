/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <common.h>
#include <uart.h>
#include "uartDefs.h"
#include "uart_hw.h"

void UARTCharPut(unsigned int baseAdd, unsigned char byteTx)
{
    unsigned int lcrRegValue = 0;

    /* Switching to Register Operational Mode of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_OPERATIONAL_MODE);

    /*
    ** Waits indefinitely until the THR and Transmitter Shift Registers are
    ** empty.
    */
    while((UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E) !=
          (DEREF(baseAdd + UART_LSR) & (UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E)));

    DEREF(baseAdd + UART_THR) = byteTx;

    /* Restoring the value of LCR. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;
}

void UART_puts(unsigned int baseAdd, char *s, unsigned int length) {
    for(unsigned int i=0; i < length; i++) {
        UARTCharPut(baseAdd, s[i]);
    }
}

signed char UARTCharGet(unsigned int baseAdd)
{
    unsigned int lcrRegValue = 0;
    signed char retVal = 0;

    /* Switching to Register Operational Mode of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_OPERATIONAL_MODE);

    /* Waits indefinitely until a byte arrives in the RX FIFO(or RHR). */
    while(0 == (DEREF(baseAdd + UART_LSR) & UART_LSR_RX_FIFO_E));

    retVal = ((signed char)DEREF(baseAdd + UART_RHR));

    /* Restoring the value of LCR. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;

    return retVal;
}
