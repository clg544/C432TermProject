/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   debug.h
 * Author: nathanhupka
 *
 * Created on March 30, 2017, 12:47 PM
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif
    
void panic();
void printUIntHex(unsigned int character);
void printUIntHexBigEndian(unsigned int character);
void printUIntHexLittleEndian(unsigned int character);
void printUIntBinaryLittleEndian(unsigned int character);
void printUIntBinaryBigEndian(unsigned int character);
int checkEndianness();
char charNum2ascii(char num);
void terminateLine();
void dumpFrame(unsigned int * frame);

static inline __attribute__((always_inline)) void* captureFrame() 
{
   asm volatile ("str sp,[sp, #-4]");
   asm volatile ("subs sp, #4");
   asm volatile ("push {r0-r7, lr}");
   asm volatile ("mov r0, sp");
   register int *r0 asm ("r0");
   return r0;
}


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */

