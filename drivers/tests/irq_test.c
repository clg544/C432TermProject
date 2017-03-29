#include <gpio.h>
#include <timer.h>
#include <common.h>
#include <interrupt.h>
#include <led.h>

extern void rtc_init();
#define RTC_BASE 0x44E3E000

volatile int irq_count;

void rtc_irq() {
    irq_count++;
}

int main() {
    int old_count = 0;
    int j = 0;
    irq_count = 0;
    /* TIMER_init(0, 1000000, 0, 0); */
    led_init();
    rtc_init();
    irq_init();
    while(1) {
        if(old_count != irq_count) {  // trigger LED on changed irq_count
              old_count = irq_count;
              j = 0;
              led_on(1);
              while (j < 200000) j++;
              led_off(1);
        }
    }

    return 0;
}
