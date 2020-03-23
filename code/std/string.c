#include <std/memory.h>
#include <std/screen.h>
#include <std/string.h>

extern unsigned short *textmemptr;
extern int attrib;
extern int csr_x, csr_y;

void putch(unsigned char c) {
	
	unsigned short *where;
	unsigned att = attrib << 8;
	
	if(c == 0x08) {
		if(csr_x != 0) csr_x--;
	}
	else if(c == 0x09) {
		csr_x = (csr_x + 8) & ~(8 - 1);
	}
	else if(c == '\r') {
		csr_x = 0;
	}
	else if(c == '\n') {
		
		csr_x = 0;
		csr_y++;
	}
	else if(c >= ' ') {
		
		where = textmemptr + (csr_y * 80 + csr_x);
		*where = c | att;
		csr_x++;
	}
	if(csr_x >= 80) {
		
		csr_x = 0;
		csr_y++;
	}
	
	scroll();
	move_csr();
}

void puts(unsigned char *text) {
	
	int i;
	
	for (i = 0; i < strlen(text); i++)
		putch(text[i]);
}