#include "uart.h"

int main() {
    /* you must explicitly declare the number
     * of characters in your strings.
     * This is flakey..*/
    char msg[4] = "test";
    UART_init();
    UART_puts(msg, 4);

    /* echo terminal! :D */
    while(1){
        UART_send_char(UART_read_char());
    }
    return 0;
}
