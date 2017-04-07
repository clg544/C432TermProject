#ifndef __GPIO__
#define __GPIO__

typedef struct GPIO GPIO;

/* Base addresses section 2.1 */
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000
#define GPIO_PORT3 0x481AE000

void GPIO_init_port(int port);

void GPIO_set_direction (unsigned int baseAdd, int dir);

void GPIO_write_port(unsigned int baseAdd,int port ,int value);

int GPIO_read_port(unsigned int baseAdd);

#endif
