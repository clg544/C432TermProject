/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timer_hw.h
 * Author: nathanhupka
 *
 * Created on March 28, 2017, 8:40 AM
 */

#ifndef TIMER_HW_H
#define TIMER_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#define DMTIMER_TIDR   (0x0)
#define DMTIMER_TIOCP_CFG   (0x10)
#define DMTIMER_IRQ_EOI   (0x20)
#define DMTIMER_IRQSTATUS_RAW   (0x24)
#define DMTIMER_IRQSTATUS   (0x28)
#define DMTIMER_IRQENABLE_SET   (0x2C)
#define DMTIMER_IRQENABLE_CLR   (0x30)
#define DMTIMER_IRQWAKEEN   (0x34)
#define DMTIMER_TCLR   (0x38)
#define DMTIMER_TCRR   (0x3C)
#define DMTIMER_TLDR   (0x40)
#define DMTIMER_TTGR   (0x44)
#define DMTIMER_TWPS   (0x48)
#define DMTIMER_TMAR   (0x4C)
#define DMTIMER_TCAR(n)   (0x50 + (((n) - 1) * 8))
#define DMTIMER_TSICR   (0x54)

#ifdef __cplusplus
}
#endif

#endif /* TIMER_HW_H */

