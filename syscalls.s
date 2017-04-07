/* Call svc using r7 as syscall id */

.global syscall
syscall:
	push {r7}       /* Save r7, we need to use it... */
	mov r7, #0x0    /* Put the Syscall code on r7 */
	svc 0           /* Signal a software interrupt to return to kernel */ 
	pop {r7}        /* kernel will put us back at this point, so reset r7 */
	bx lr           /* Back to our program */

.global fork
fork:
	push {r7}
	mov r7, #0x1
	svc 0
	pop {r7}
	bx lr

.global get_pid
get_pid:
	push {r7}
	mov r7, #0x2
	svc 0
	pop {r7}
	bx lr

.global get_parent_pid
get_parent_pid:
	push {r7}
	mov r7, #0x3
	svc 0
	pop {r7}
	bx lr

.global end
end:
	push {r7}
	mov r7, #0x4
	svc 0
	pop {r7}
	bx lr

.global wait_pid
wait_pid:
	push {r7}
	mov r7, #0x5
	svc 0
	pop {r7}
	bx lr

.global print
print:
	push {r7}
	mov r7, #0x6
	svc 0
	pop {r7}
	bx lr