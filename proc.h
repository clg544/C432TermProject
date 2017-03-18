
struct process* proc_table;

struct processor_state { 
  int r0;
  int r1;
  int r2;
  int r3;
  int r4;
  int r5;
  int r6;
  int r7;
  int r8;
  int r9;
  int r10;
  int r11;
  int r12;
  int r13;
  int r14;
  int r15;  
}__attribute__((packed)) 

struct process{
  struct processor_state;
  unsigned int pid;
  
  
  
}