.equ STACK_SIZE, 256
.global _start
_start:
   mrs r0, cpsr
   bic r0, r0, #0x1F @ clear mode bits
   orr r0, r0, #0x13 @ set SVC mode
   orr r0, r0, #0xC0 @ disable FIQ and IRQ
   msr cpsr, r0

  ldr r0, =0x4030CE28
  ldr r1, =interrupt_address_table
  ldr r3, =interrupt_address_table_end
keep_loading:
    ldr r2, [r1, #0x0]
    str r2, [r0, #0x0]
    add r0, r0, #0x4
    add r1, r1, #0x4
    cmp r1, r3
    bne keep_loading
_disable_mmu:
    mrc p15, 0, r1, c1, c0, 0
    bic r1, r1, #0x1
    mcr p15, 0, r1, c1, c0, 0
_diable_l1_caches:
    mrc p15, 0, r1, c1, c0, 0
    bic r1, r1, #(0x1 << 12)
    bic r1, r1, #(0x1 << 2)
    mcr p15, 0, r1, c1, c0, 0
_invalidate_i_cache:
    mov r1, #0
    mcr p15, 0, r1, c7, c5, 0
_invalidate_d_cache:
    mrc p15, 1, r0, c0, c0, 0
    ldr r3, =0x1ff
    and r0, r3, r0, lsr #13

    mov r1, #0
    way_loop:
    mov r3, #0
    set_loop:
    mov r2, r1, LSL #30
    orr r2, r3, LSL #5
    mcr p15, 0, r2, c7, c6, 2
    add r3, r3, #1
    cmp r0, r3
    bgt set_loop
    add r1, r1, #1
    cmp r1, #4
    bne way_loop

    @ Invalidate TLB
    mcr p15, 0, r1, c8, c7, 0

    @ Branch Prediction Enable
    mov r1, #0
    mrc p15, 0, r1, c1, c0, 0
    orr r1, r1, #(0x1 << 11)
    mcr p15, 0, r1, c1, c0, 0

    /* setup stack pointer */
    /*
    ldr sp, =0x4030CDFC
    sub r1, sp, #STACK_SIZE
    */

    /* save svc */
    mrs r3, cpsr

    /*
    mov r2, #0x1b @undef
    msr cpsr_cxsf, r2
    mov sp, r1
    sub r1, sp, #STACK_SIZE

    mov r2, #0x12 @irq
    msr cpsr_cxsf, r2
    mov sp, r1
    sub r1, sp, #STACK_SIZE
    */

    bic r3, r3, #0x80 /* enable interrupts */

    /* return to svc */
    msr cpsr_cxsf, r3

    /*mov r3, r0 */
    /* zero out bss */
    /*
    ldr     r0, =__bss_start__
    ldr     r1, =__bss_size__
    add     r1, r0
    mov     r2, #0
0:
    cmp     r0, r1
    strlt   r2, [r0], #4
    blt     0b

    mov     r0, r3
    */
    b       main

.data
interrupt_address_table:
.word 0x00000
.word 0x20088
.word 0x2008C
.word 0x20090
.word irq_entry
.word fiq_entry
interrupt_address_table_end:

/*
The MIT License (MIT)

Copyright (c)  Alexis Marquet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
