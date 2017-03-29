/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   intc_hw.h
 * Author: nathanhupka
 *
 * Created on March 28, 2017, 1:34 PM
 */

#ifndef INTC_HW_H
#define INTC_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#define INTC_REVISION (0x0)
#define INTC_SYSCONFIG (0x10)
#define INTC_SYSSTATUS (0x14)
#define INTC_SIR_IRQ (0x40)
#define INTC_SIR_FIQ (0x44)
#define INTC_CONTROL (0x48)
#define INTC_PROTECTION (0x4c)
#define INTC_IDLE  (0x50)
#define INTC_IRQ_PRIORITY (0x60)
#define INTC_FIQ_PRIORITY (0x64)
#define INTC_THRESHOLD (0x68)
#define INTC_SICR  (0x6c)
#define INTC_SCR(n) (0x70 + ((n) * 0x04))
#define INTC_ITR(n) (0x80 + ((n) * 0x20))
#define INTC_MIR(n) (0x84 + ((n) * 0x20))
#define INTC_MIR_CLEAR(n) (0x88 + ((n) * 0x20))
#define INTC_MIR_SET(n) (0x8c + ((n) * 0x20))
#define INTC_ISR_SET(n) (0x90 + ((n) * 0x20))
#define INTC_ISR_CLEAR(n) (0x94 + ((n) * 0x20))
#define INTC_PENDING_IRQ(n) (0x98 + ((n) * 0x20))
#define INTC_PENDING_FIQ(n) (0x9c + ((n) * 0x20))
#define INTC_ILR(n)  (0x100 + ((n) * 0x04))

#ifdef __cplusplus
}
#endif

#endif /* INTC_HW_H */

