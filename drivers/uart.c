#include "uart.h"
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

void UART_init() {
    unsigned int div = 48000000.0/(16.0*115200);
    DEREF(CONTROL_MODULE+conf_uart0_rxd) = 1<<4|1<<5;
    DEREF(CONTROL_MODULE+conf_uart0_txd) = 0;

    DEREF(CM_WKUP) &= ~0x3; /* reset bits */
    DEREF(CM_WKUP) |= 0x2; /* Start software forced wake-up */
    /* This isn't needed apparently? */
    /* DEREF(0x44E0011C) &= 0x3; //CM_PER_L4HS_CLKSTSTRL */
    /* DEREF(0x44E0011C) |= 0x2; */
    /* DEREF(0x44E004B4) &= 0x3; //CM_WKUP_UART0_CLKCTRL */
    /* DEREF(0x44E004B4) |= 0x2; */
    /* DEREF(0x44E0006C) &= 0x3; //CM_PER_UART0_CLKCTRL */
    /* DEREF(0x44E0006C) |= 0x2; */

    UART_soft_reset(0);
    DEREF(UART_PORT0 + UART_SYSC) |= 0x8; /* disable smart idle */
    while((DEREF(UART_PORT0+LSR_UART)&0x40) != 0x40);/*wait for transmitter hold register to be empty */
    DEREF(UART_PORT0+0x04) = 0;
    DEREF(UART_PORT0+UART_MDR1) = 7; /* disable UART during config*/
    DEREF(UART_PORT0+UART_LCR) = ~0x7C;
    DEREF(UART_PORT0+0x0) = 0;
    DEREF(UART_PORT0+0x04) = 0;
    DEREF(UART_PORT0+UART_LCR) = 0x3;
    DEREF(UART_PORT0+UART_MCR) = 0x3; /* force RTS and DTR to active */
    DEREF(UART_PORT0+0x08) = 0x7;
    DEREF(UART_PORT0+UART_LCR) = ~0x7C;
    /* load frequency divider for baudrate control */
    DEREF(UART_PORT0+UART_DLL) = div;
    DEREF(UART_PORT0+UART_DLH) = (div>>8)&0x3F;

    DEREF(UART_PORT0+UART_LCR) = 0x3; /*8bit word length */
    DEREF(UART_PORT0+UART_MDR1) = 0; /* enable UART 16x mode */

    /* Doesn't work unless you print 11 identical
     * characters. I have no idea why... :(
     */
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char(' ');
    UART_send_char('\r'); /* go to next line for prettynes */
    UART_send_char('\n');

}

void UART_send_char(char c) {
    /* wait for transmit register to be empty */
    while((DEREF8(UART_PORT0+LSR_UART)&0x20) != 0x20){}
    DEREF8(UART_PORT0) = c;

}

char UART_read_char() {
    /* wait for datda in receive fifo */
    while((DEREF8(UART_PORT0+LSR_UART)&0x1) == 0){}
    return DEREF8(UART_PORT0);
}

void UART_puts(char *s, unsigned int length) {
    for(int i=0; i<length; i++) {
        UART_send_char(s[i]);
    }
}
