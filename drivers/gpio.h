#ifndef __GPIO__
#define __GPIO__

/* Base addresses section 2.1 */
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000
#define GPIO_PORT3 0x481AE000

/* Offsets section 25.4.1 */
#define GPIO_OE      0x134
#define GPIO_DATAIN  0x138
#define GPIO_DATAOUT 0x13C


void GPIO_init_port(int port);

void GPIO_set_direction(int port, int dir);

void GPIO_write_port(int port, int value);

int GPIO_read_port(int port);

#endif
