#include <common/multiboot.h>
#include <std/screen.h>
#include <std/string.h>
#include <hw/port.h>
#include <hw/timer.h>
#include <descriptors/gdt.h>
#include <descriptors/idt.h>
#include <interrupts/isr.h>
#include <interrupts/irq.h>
#include <driver/keyboard.h>

/* These define our textpointer, our background and foreground
 *  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
	textmemptr = (unsigned short *)0xB8000;
	cls();
}

void kernelMain(struct multiboot_info* mbt, addr stack) {
    
	gdt_install();
	idt_install();
	isr_install();
	irq_install();
	init_video();
	timer_install();
	keyboard_install();
	
	__asm__ __volatile__ ("sti");
	
	puts("Kein Betriebssystem gefunden...\n");
	
    while(1);
}