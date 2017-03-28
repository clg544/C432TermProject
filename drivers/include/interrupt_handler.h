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

void svc_entry(void);
void irq_entry(void);
void fiq_entry(void);

#ifdef __cplusplus
}
#endif

#endif /* INTERRRUPT_HANDLER_H */

