/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   interrrupt_handler.h
 * Author: nathanhupka
 *
 * Created on March 24, 2017, 12:19 PM
 */

#ifndef INTERRRUPT_HANDLER_H
#define INTERRRUPT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define INT_CONTROLLER    0x48200000

#define INTC_SYSCONFIG 0x10
#define INTC_SYSSTATUS 0x14
#define INTC_SIR_IRQ 0x40
#define INTC_SIR_FIQ 0x44
#define INTC_CONTROL 0x48
#define INTC_MIR_CLEAR(x) (0x88+((x)*0x20))
#define INTC_MIR_SET(x) (0x8c + ((x)*0x20))

#define VECT_UND 0x4030CE24 /* undefined instruction RAM exception vector */
#define VECT_IRQ 0x4030CE38 /* IRQ RAM exception vector */
#define VECT_FIQ 0x4030CE3C

void svc_entry(void);
void irq_entry(void);
void fiq_entry(void);

void irq_init();
void rtc_irq();

#ifdef __cplusplus
}
#endif

#endif /* INTERRRUPT_HANDLER_H */

