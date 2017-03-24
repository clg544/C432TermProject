.equ STACK_SIZE, 256
_start:
    /* setup stack pointer */
    ldr sp, =0x4030CDFC
    sub r1, sp, #STACK_SIZE

    /* save svc */
    mrs r3, cpsr

    mov r2, #0x1b @undef
    msr cpsr_cxsf, r2
    mov sp, r1
    sub r1, sp, #STACK_SIZE

    mov r2, #0x12 @irq
    msr cpsr_cxsf, r2
    mov sp, r1
    sub r1, sp, #STACK_SIZE

    /* return to svc */
    msr cpsr_cxsf, r3

    mov r3, r0
    /* zero out bss */
    ldr     r0, =__bss_start__
    ldr     r1, =__bss_size__
    add     r1, r0
    mov     r2, #0
0:
    cmp     r0, r1
    strlt   r2, [r0], #4
    blt     0b

    mov     r0, r3
    b       main

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
