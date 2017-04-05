#include "../HAL/device.h"
#include <interrupt_handler.h>

int main() {
    irq_init();
    enumerate_devices();

    struct device *led = get_device(LED);
    struct device *serial = get_device(SERIAL);
    led->write(5);
    serial->write('a');

    while(1) {
        serial->write(serial->read());
    }

    return 0;
}


