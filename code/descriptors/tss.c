#include <descriptors/tss.h>

tss_t sys_tss;

void tss_set_kernel_stack(unsigned int stack) {
	
	sys_tss.esp0 = stack;
}

void tss_install(signed int num, unsigned short ss0, unsigned short esp0) {

	addr base = (addr)&sys_tss;
	addr size = base + sizeof(tss_t);
	
	gdt_set_gate(num, base, size, 0xE9, 0x00);
	
	memset(&sys_tss, 0, sizeof(sys_tss));
	
	sys_tss.ss0	= ss0;
	sys_tss.esp0 = esp0;
	
	sys_tss.cs = 0x0B;
	sys_tss.ss = sys_tss.ds = sys_tss.es = sys_tss.fs = sys_tss.gs = 0x13;
}

extern volatile struct task* current_task;

void tss_switch() {
	
	tss_set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);
	
	asm volatile("\
	cli; \
	mov $0x23, %ax; \
	mov %ax, %ds; \
	mov %ax, %es; \
	mov %ax, %fs; \
	mov %ax, %gs; \
	\
	mov %esp, %eax; \
	pushl $0x23; \
	pushl %eax; \
	pushf; \
	\
	pop %eax; \
	or $0x200, %eax; \
	push %eax; \
	\
	pushl $0x1B; \
	push $1f; \
	iret; \
	1: \
	");
}