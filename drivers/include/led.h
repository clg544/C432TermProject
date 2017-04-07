/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   led.h
 * Author: nathanhupka
 *
 * Created on March 27, 2017, 4:50 PM
 */

#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void led_init();
    void led_off(int led);
    void led_on(int led);


#ifdef __cplusplus
}
#endif

#endif /* LED_H */

