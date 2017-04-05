#include "device.h"

/* From drivers/include */
#include <uart.h>
#include <led.h>
#include <gpio.h>

static struct device devices[2];// UART0 and LEDs


/* Default functions */
int default_write(char data) {
    return -1;
}

char default_read() {
    return -1;
}

int default_open() {
    return -1;
}

int default_close() {
    return -1;
}


/* UART serial functions */
int uart_write(char data) {
    UARTCharPut(UART_PORT0, data);
    return 0;
}

char uart_read() {
    char c = UARTCharGet(UART_PORT0);
    return c;
}



/* LED functions */
int led_write(unsigned int byte) {
    GPIO_write_port(GPIO_PORT1, 1, byte<<20);
    return 0;
}




void enumerate_devices() {
    UART_Init(0);
    devices[0].type = SERIAL;
    devices[0].write = uart_write;
    devices[0].read = uart_read;
    devices[0].open = default_open;
    devices[0].close = default_close;
    devices[0].instance = 0;

    led_init();
    devices[1].type = LED;
    devices[1].write = led_write;
    devices[1].read = default_read;
    devices[1].open = default_open;
    devices[1].close = default_close;
    devices[1].instance = 0;

}


/* I would like to change the integer dev
 * to a string to make this function easier to use.
 */
struct device *get_device(enum device_type dev) {
    if(dev == SERIAL) {
        return &devices[SERIAL];
    } else if(dev == LED) {
        return &devices[LED];
    } else {
        return 0;
    }
}


