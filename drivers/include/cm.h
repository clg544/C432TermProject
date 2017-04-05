/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cm.h
 * Author: nathanhupka
 *
 * Created on March 30, 2017, 10:38 AM
 */

#ifndef CM_H
#define CM_H

#ifdef __cplusplus
extern "C" {
#endif

void UARTPinMuxSetup(unsigned int instanceNum);
void UART0ModuleClkConfig(void);


#ifdef __cplusplus
}
#endif

#endif /* CM_H */

