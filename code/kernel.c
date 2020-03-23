#include <common/multiboot.h>
#include <std/screen.h>
#include <std/string.h>
#include <hw/port.h>
#include <hw/serial.h>
#include <hw/timer.h>
#include <descriptors/gdt.h>
#include <descriptors/idt.h>
#include <interrupts/isr.h>
#include <interrupts/irq.h>
#include <driver/keyboard.h>

unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

void init_video(void) {
	
	textmemptr = (unsigned short *)0xB8000;
	cls();
}

void kernelMain(struct multiboot_info* mbt, addr stack) {
    
	gdt_install();
	idt_install();
	isr_install();
	irq_install();
	//init_serial(COM1);
	init_video();
	timer_install();
	keyboard_install();
	
	__asm__ __volatile__("sti");
	
	puts("Keine Betriebssystem gefunden...Beruder\n");
	
	/*
	write_serial(COM1, 0x41);
	write_serial(COM1, 0x42);
	write_serial(COM1, 0x43);
	putch(read_serial(COM1));
	putch(read_serial(COM1));
	putch(read_serial(COM1));
	*/
	
    while(1) {
		
		//puts("Ajj warte Beruder\n");
		
		//timer_wait(18);
	}
}