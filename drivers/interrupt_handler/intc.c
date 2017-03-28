/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 
 */


INTC* intc = (INTC*)0x48200000;

void int_intc(){
    intc->INTC_IDLE = 0x1;
}
