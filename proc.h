
struct task_struct* task_list;

struct cpu_registers {
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

enum task_state{
    initial,    // This is the initial state when a process is first started/created.
    ready,      // Task is not currently running but is ready to run.
    running,    // Currently executing.
    waiting,    // Not running, and not able to run; presumably until some blocking operation completes.
    zombie      // Task has exited and is waiting to have its return code harvested by it's parent
};

struct task_struct{
    struct cpu_registers;
    unsigned int pid;
    task_state state;
}
