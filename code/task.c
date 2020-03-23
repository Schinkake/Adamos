#include <descriptors/tss.h>
#include <frame.h>
#include <task.h>
#include <page.h>

volatile struct task* current_task;

volatile struct task* ready_queue;

extern addr initial_esp;
extern addr read_eip();

unsigned int next_pid = 1;

void move_stack(void* new_stack_start, addr size) {
	
	addr i;
	
	for (i = (addr)new_stack_start; i >= ((addr)new_stack_start-size); i -= 0x1000)
		frame_alloc((struct page*)get_page(i, 1, current_directory), 0, 1);
	
	addr pd_addr;
	asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
	asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

	addr old_stack_pointer;
	addr old_base_pointer;
	asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
	asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));
	addr offset = (addr)new_stack_start - initial_esp;
	addr new_stack_pointer = old_stack_pointer + offset;
	addr new_base_pointer = old_base_pointer + offset;
	
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp - old_stack_pointer);

	for (i = (addr)new_stack_start; i > (addr)new_stack_start - size; i -= 4) {
		
		addr tmp = *(addr*)i;
		
		if ((old_stack_pointer < tmp) && (tmp < initial_esp)) {
			
			tmp = tmp + offset;
			addr* tmp2 = (addr*)i;
			*tmp2 = tmp;
		}
	}
	
	asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
	asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

int fork() {
	
	asm volatile("cli");
	
	struct task* parent_task = (struct task*)current_task;
	
	struct page_directory* directory = (struct page_directory*)clone_directory(current_directory);
	
	struct task* new_task = (struct task*)kmalloc(sizeof(struct task));
	new_task->id = next_pid++;
	new_task->esp = new_task->ebp = 0;
	new_task->eip = 0;
	new_task->page_directory = directory;
	new_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);
	new_task->next = 0;
	
	struct task* tmp_task = (struct task*)ready_queue;
	while (tmp_task->next)
		tmp_task = tmp_task->next;

	tmp_task->next = new_task;
	
	addr eip = read_eip();
	
	if (current_task == parent_task) {
		
		addr esp; asm volatile("mov %%esp, %0" : "=r"(esp));
		addr ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
		new_task->esp = esp;
		new_task->ebp = ebp;
		new_task->eip = eip;
		
		asm volatile("sti");
		
		return new_task->id;
	}
	else
		return 0;
}

int getpid() {
	
	return current_task->id;
}

void task_switch() {
	
	if (!current_task)
		return;
	
	addr esp, ebp, eip;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));
	
	eip = read_eip();
	
	if (eip == 0x12345)
		return;
	
	current_task->eip = eip;
	current_task->esp = esp;
	current_task->ebp = ebp;
	
	current_task = current_task->next;
	if (!current_task) current_task = ready_queue;
	
	eip = current_task->eip;
	esp = current_task->esp;
	ebp = current_task->ebp;
	
	current_directory = current_task->page_directory;
	
	tss_set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);
	
	asm volatile("				\
	cli;			\
	mov %0, %%ecx;		\
	mov %1, %%esp;		\
	mov %2, %%ebp;		\
	mov %3, %%cr3;		\
	mov $0x12345, %%eax;	\
	sti;			\
	jmp *%%ecx		"
	: : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->phys_addr));
}

void task_install() {
	
	puts("Enabling multitasking... ");
	
	asm volatile("cli");
	
	move_stack((void*)0xE0000000, 0x2000);
	
	current_task = ready_queue = (struct task*)kmalloc(sizeof(struct task));
	current_task->id = next_pid++;
	current_task->esp = current_task->ebp = 0;
	current_task->eip = 0;
	current_task->page_directory = current_directory;
	current_task->next = 0;
	current_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);
	
	asm volatile("sti");
	
	puts("done.\n");
}