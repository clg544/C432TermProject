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
    devices[SERIAL].type = SERIAL;
    devices[SERIAL].write = uart_write;
    devices[SERIAL].read = uart_read;
    devices[SERIAL].open = default_open;
    devices[SERIAL].close = default_close;
    devices[SERIAL].instance = 0;

    led_init();
    devices[LED].type = LED;
    devices[LED].write = led_write;
    devices[LED].read = default_read;
    devices[LED].open = default_open;
    devices[LED].close = default_close;
    devices[LED].instance = 0;

}


struct device *get_device(enum device_type dev) {
    if(dev == SERIAL) {
        return &devices[SERIAL];
    } else if(dev == LED) {
        return &devices[LED];
    } else {
        return 0;
    }
}


