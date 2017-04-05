#include "../HAL/device.h"

int main() {
    enumerate_devices();

    struct device *led = get_device(1);
    led->write(5);

    while(1) {

    }

    return 0;
}


