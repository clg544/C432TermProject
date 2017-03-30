/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <led.h>
void panic(){
    led_on(1);
    led_on(2);
    led_on(3);
    led_on(4);
    while(1);
}