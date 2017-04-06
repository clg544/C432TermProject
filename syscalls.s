/* Call svc using r7 as syscall id */

.global syscall
syscall:
	push {r7}
	mov r7, #0x0
	svc 0
	pop {r7}
	bx lr

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
	mov r7, #0x4
	svc 0
	pop {r7}
	bx lr


