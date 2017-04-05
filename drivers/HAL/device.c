#include "device.h"

/* From drivers/include */
#include <uart.h>
#include <led.h>
#include <gpio.h>

static struct device devices[2];// UART0 and LEDs
static int device_proc[2]; // stores which PID has a given device


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
    if(device_proc[SERIAL] == 0) {
        return -1;
    } else {
        UARTCharPut(UART_PORT0, data);
        return 0;
    }
}

char uart_read() {
    if(device_proc[SERIAL] == 0) {
        return -1;
    } else {
        char c = UARTCharGet(UART_PORT0);
        return c;
    }
}

int uart_open() {
    if(device_proc[SERIAL] == 0) {
        device_proc[SERIAL] = 1; // TODO: change to PID that is trying to acces device
        return 0;
    } else {
        return -1;
    }
}

int uart_close() {
    if(device_proc[SERIAL] == 1) {
        device_proc[SERIAL] = 0;
        return 0;
    } else {
        return -1;
    }
}



/* LED functions */
int led_write(unsigned int byte) {
    if(device_proc[LED] == 0) {
        return -1;
    } else {
        GPIO_write_port(GPIO_PORT1, 1, byte<<20);
        return 0;
    }
}

int led_open() {
    if(device_proc[LED] == 0) {
        device_proc[LED] = 1; // TODO: change to PID that is trying to acces device
        return 0;
    } else {
        return -1;
    }
}

int led_close() {
    if(device_proc[LED] == 1) {
        device_proc[LED] = 0;
        return 0;
    } else {
        return -1;
    }
}



void enumerate_devices() {
    device_proc[SERIAL] = 0;
    UART_Init(0);
    devices[SERIAL].type = SERIAL;
    devices[SERIAL].write = uart_write;
    devices[SERIAL].read = uart_read;
    devices[SERIAL].open = uart_open;
    devices[SERIAL].close = uart_close;
    devices[SERIAL].instance = 0;

    device_proc[LED] = 0;
    led_init();
    devices[LED].type = LED;
    devices[LED].write = led_write;
    devices[LED].read = default_read;
    devices[LED].open = led_open;
    devices[LED].close = led_close;
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


