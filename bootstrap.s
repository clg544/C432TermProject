.global _start
_start:
	mov r0, #0x08
	ldr r1, =interrupt_table
	ldr r3, =interrupt_table_end
/* Loop: Load interrupt table vector */
keep_loading:
	ldr r2, [r1, #0x0]
	str r2, [r0, #0x0]
	add r0, r0, #0x4
	add r1, r1, #0x4
	cmp r1, r3
	bne keep_loading	/* != interrupt_table_end */

	ldr sp, =0x07FFFFFF
	bl main

interrupt_table:
	ldr pc, svc_entry_address
	nop
	nop
	nop
	ldr pc, irq_entry_address
	svc_entry_address: .word svc_entry
	irq_entry_address: .word irq_entry
interrupt_table_end: