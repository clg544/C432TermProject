#include "uart.h"
#include "gpio.h"

int main() {

    GPIO_init_port(1);
    GPIO_set_direction(1, 1<<21);
    UART_init(0);
    GPIO_write_port(1, 1<<21);
    UART_send_char(0, 'a');

    while(1){}
    return 0;
}
