#include <led.h>
#include <timer.h>

unsigned int dmtimer0 = 0;

void led_blink(){
    led_on(1);
    TIMER_delay(0, 1000);
    led_off(1);
    TIMER_delay(0, 1000);
}


int main() {
    /* int i; */
    TIMER_init(0, 1000000, 0, 0);
    led_init();
    while(1) {
        led_blink();
    }
    return 0;
}
