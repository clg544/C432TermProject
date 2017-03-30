#include <led.h>
#include <timer.h>

unsigned int dmtimer0 = DMTIMER0;

void led_blink(){
    led_on(1);
    TIMER_delay(dmtimer0, 0xFFFF);
    led_off(1);
}


int main() {
    /* int i; */
    TIMER_init(dmtimer0, 1000000, 0, 0);
    led_init();
    while(1) {
	led_blink();
    }
    return 0;
}
