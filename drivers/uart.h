#ifndef __UART__
#define __UART__

/* Base addresses section 2.1 */
#define UART_PORT0 0x44E09000

/* Offsets section 19.5.1 */
#define UART_DLL  0x00 /* Divisor Latches Low Register */
#define UART_DLH  0x04 /* Divisor Latches High Register */
#define IER_UART  0x04 /* Interrupt Enable Reginster */
#define UART_FCR  0x08 /* FIFO Control Register */
#define UART_MCR  0x10 /* Modem Control Register */
#define UART_LCR  0x0C /* Line Control Register */
#define LSR_UART  0x14 /* Line status register */
#define UART_MDR1 0x20 /* Mode Definition Register 1*/
#define UART_SYSC 0x54 /* System configuration register */
#define UART_SYSS 0x58 /* System status register */


/* Performs a software reset on the desired port */
void UART_soft_reset(unsigned int port);

void UART_init();

void UART_send_char(char c);

char UART_read_char();


#endif
