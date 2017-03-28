/* SPSR:SAVED PROCESS STATUS REGISTER */
/* CPSR CURRENT PROCESSOR STATUS REGISTER */

/* To Kernel */
.global svc_entry
svc_entry:
	/* Save user state */
	msr CPSR_c, #0xDF	/* System Mode */
	push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}
	mov r0, sp
	msr CPSR_c, #0xD3	/* Supervisor Mode */

	mrs ip, SPSR
	stmfd r0!, {ip,lr}

	/* Load kernel state */
	pop {r4,r5,r6,r7,r8,r9,r10,fp, ip,lr}
	mov sp,ip
	bx lr

/* To User */
.global activate	/* activate(r0:uint stack_pointer) */ 
activate:
	
	/* Save kernel state */
	mov ip, sp
	push {r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}

	ldmfd r0!, {ip,lr} 	/* Get SPSR and lr */
	msr SPSR, ip

	msr CPSR_c, #0xDF	/* System Mode */
	mov sp, r0		/* Stack pointer of new process, as arg_0 */
	pop {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}
				/* Pop values off stack */
	msr CPSR_c, #0xD3 	/* Supervisor Mode */

	movs pc, lr

/* Entry point for hardware interrupts */
.global irq_entry
irq_entry:
	msr CPSR_c, #0xDf	/* System Mode */

	push {r7}
	ldr r7, PIC		/* Load PIC Status */
	ldr r7, [r7]
	PIC: .word 0x10140000
	clz r7, r7
	sub r7, r7, #31 
	
	/* Save User State */ 
	push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}
	mov r0, sp
	
	msr CPSR_c, #0xD2 /* IRQ mode */
	mrs ip, SPSR
	sub lr, lr, #0x4 /* lr is address of next instruction */
	stmfd r0!, {ip,lr}
	
	msr CPSR_c, #0xD3 /* Supervisor mode */
	
	/* Load kernel state */
	pop {r4,r5,r6,r7,r8,r9,r10,fp,ip,lr}
	mov sp, ip
	bx lr
