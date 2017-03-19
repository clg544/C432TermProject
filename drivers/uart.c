#include "uart.h"
#include "gpio.h"
#include "clock.h"
#include "common.h"

void UART_soft_reset(unsigned int port) {
    switch(port) {
        case 0:
            DEREF(UART_PORT0 + UART_SYSC) |= 0x1; /* signal reset */
            while((DEREF(UART_PORT0 + UART_SYSS)&0x1) == 0){} /*wait for reset completion*/
            break;
        default:
            break;
    }

}

void UART_init(unsigned int port) {
    float div = 48000000.0/(16.0*115200);
    unsigned int intdiv = *((unsigned int*)&div);
    GPIO_write_port(1, 1<<22);
    DEREF(CONTROL_MODULE+conf_uart0_rxd) = 1<<4|1<<5;
    DEREF(CONTROL_MODULE+conf_uart0_txd) = 0;

    DEREF(CM_WKUP) &= ~0x3; /* reset bits */
    DEREF(CM_WKUP) |= 0x2; /* Start software forced wake-up */
    DEREF(0x44E0011C) &= 0x3; //CM_PER_L4HS_CLKSTSTRL
    DEREF(0x44E0011C) |= 0x2;
    DEREF(0x44E004B4) &= 0x3; //CM_WKUP_UART0_CLKCTRL
    DEREF(0x44E004B4) |= 0x2;
    DEREF(0x44E0006C) &= 0x3; //CM_PER_UART0_CLKCTRL
    DEREF(0x44E0006C) |= 0x2;

    UART_soft_reset(0);
    DEREF(UART_PORT0 + UART_SYSC) |= 0x8; /* disable smart idle */
    while((DEREF(UART_PORT0+LSR_UART)&0x40) != 0x40);
    DEREF8(UART_PORT0+0x04) = 0;
    DEREF8(UART_PORT0+0x20) = 7;
    DEREF8(UART_PORT0+0x0C) = ~0x7C;
    DEREF8(UART_PORT0+0x0) = 0;
    DEREF8(UART_PORT0+0x04) = 0;
    DEREF8(UART_PORT0+0x0C) = 0x3;
    DEREF8(UART_PORT0+0x10) = 0x3;
    DEREF8(UART_PORT0+0x08) = 0x7;
    DEREF8(UART_PORT0+0x0C) = ~0x7C;
    DEREF8(UART_PORT0+0x0) = intdiv&0xFF;
    DEREF8(UART_PORT0+0x04) = (intdiv>>8)&0x3F;

    DEREF8(UART_PORT0+0x0C) = 0x3;
    DEREF8(UART_PORT0+0x20) = 0;

}

void UART_send_char(unsigned int port, char c) {
    while((DEREF8(UART_PORT0+0x14)&0x20) != 0x20);
    DEREF8(UART_PORT0) = c;

}

char UART_read_char(unsigned int port) {

    return '\0';
}
