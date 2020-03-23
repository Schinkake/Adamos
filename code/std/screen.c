#include <std/memory.h>
#include <std/screen.h>
#include <hw/port.h>

extern unsigned short *textmemptr;
extern int attrib;
extern int csr_x, csr_y;

void scroll(void) {
	
	unsigned blank, temp;
	
	blank = 0x20 | (attrib << 8);
	
	if(csr_y >= 25) {

		temp = csr_y - 25 + 1;
		memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);
		
		memsetw (textmemptr + (25 - temp) * 80, blank, 80);
		csr_y = 25 - 1;
	}
}

void move_csr(void) {
	
	unsigned temp;
	
	temp = csr_y * 80 + csr_x;
	
	port8BitWrite(0x3D4, 14);
	port8BitWrite(0x3D5, temp >> 8);
	port8BitWrite(0x3D4, 15);
	port8BitWrite(0x3D5, temp);
}

void cls() {
	
	unsigned blank;
	int i;
	
	blank = 0x20 | (attrib << 8);
	
	for(i = 0; i < 25; i++)
		memsetw (textmemptr + i * 80, blank, 80);
	
	csr_x = 0;
	csr_y = 0;
	move_csr();
}