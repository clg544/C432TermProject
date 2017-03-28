/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <led.h>
#include <gpio.h>

unsigned int gpio1 = GPIO_PORT1;

void led_init(){
    GPIO_init_port(1);
    GPIO_set_direction(gpio1, 0);
}

void led_off(int led){
    GPIO_write_port(gpio1, (20 + led) , 0);
}

void led_on(int led){
    GPIO_write_port(gpio1, (20 + led) , 1);
}