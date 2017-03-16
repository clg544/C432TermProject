#include "uart.h"
#include "gpio.h"
#include "common.h"

void UART_soft_reset(unsigned int port) {
    switch(port) {
        case 0:
            DEREF(UART_PORT0 + UART_SYSC) |= 0x1; /* signal reset */
            while(DEREF(UART_PORT0 + UART_SYSS)&0x1 == 0){} /*wait for reset completion*/
            break;
        default:
            break;
    }

}

void UART_init(unsigned int port) {
    switch(port) {
        case 0:
            DEREF(CM_WKUP) &= ~0x3; /* reset bits */
            DEREF(CM_WKUP) |= 0x2; /* Start software forced wake-up */
            UART_soft_reset(0);
            DEREF(UART_PORT0 + UART_SYSC) |= 0x8; /* disable smart idle */
            break;
        default:
            break;
    }

}

void UART_send_char(unsigned int port, char c) {

}

char UART_read_char(unsigned int port) {

    return '\0';
}
