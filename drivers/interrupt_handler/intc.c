/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 
 */

#include <common.h>
unsigned int intc = (INTC*)0x48200000;

void int_intc(){
    unsigned int mask = ~(0x1);
    DEREF(intc + INTC_SYSCONFIG) = DEREF(intc + INTC_SYSCONFIG) & mask;
    unsigned int mask = ~(0x2);
    DEREF(intc + INTC_IDLE) = DEREF(intc + INTC_IDLE) & mask;
    
}

void clear_mir_line(int i){
    unsigned int reg = (i + 1) / 32;
    unsigned int bit = 32 - ((i + 1) % 32);
    unsigned int mask = (1 << bit);
    (intc + INTC_MIR_CLEAR(reg)) = mask;
}

void set_mir_line(int i){
    unsigned int reg = (i + 1) / 32;
    unsigned int bit = 32 - ((i + 1) % 32);
    unsigned int mask = (1 << bit);
    (intc + INTC_MIR_SET(reg)) = mask;
}
