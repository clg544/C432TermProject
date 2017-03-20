#include "uart.h"

int main() {
    UART_init();

    /* echo terminal! :D */
    while(1){
        UART_send_char(UART_read_char());
    }
    return 0;
}
