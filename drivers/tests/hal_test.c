#include "../HAL/device.h"

int main() {
    enumerate_devices();

    struct device *led = get_device(LED);
    struct device *serial = get_device(SERIAL);
    led->write(5);

    while(1) {
        serial->write(serial->read());
    }

    return 0;
}


