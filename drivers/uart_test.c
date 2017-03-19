#include "uart.h"
#include "gpio.h"

int main() {

    GPIO_init_port(1);
    GPIO_set_direction(1, (1<<21)|(1<<22));
    UART_init();
    UART_send_char('b');
    GPIO_write_port(1, 1<<21);

    while(1){}
    return 0;
}
