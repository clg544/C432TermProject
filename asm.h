/* pc(r15): Program Counter
 * lr(r14): Link Register
 * sp(r13): Stack Pointer
 * ip(r12): Intra-Procedure Scratch Register 
 */

/* do a context switch */
unsigned int *activate(unsigned int*);

/* Activate the svc interrupt instruction */
void syscall(void);	
int fork(void);	
