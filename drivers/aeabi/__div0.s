.extern panic
.text
	.globl	 __div0
	.type __div0      ,function
	.align	0
__div0  :
    ldr r3, =panic
    bx r3


