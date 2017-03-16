#ifndef __UART__
#define __UART__

/* Base addresses section 2.1 */
#define UART_PORT0 0x44E0_9000

/* Offsets section 19.5.1 */
#define UART_SYSC 0x54 /* System configuration register */
#define UART_SYSS 0x58 /* System status register */


/* Performs a software reset on the desired port */
void UART_soft_reset(unsigned int port);

void UART_init(unsigned int port);

void UART_send_char(unsigned int port, char c);

char UART_read_char(unsigned int port);


#endif
