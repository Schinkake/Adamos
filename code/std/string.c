#include <std/memory.h>
#include <std/screen.h>
#include <std/string.h>

/* These define our textpointer, our background and foreground
 *  colors (attributes), and x and y cursor coordinates */
extern unsigned short *textmemptr;
extern int attrib;
extern int csr_x, csr_y;

void putch(unsigned char c)
{
	unsigned short *where;
	unsigned att = attrib << 8;
	
	/* Handle a backspace, by moving the cursor back one space */
	if(c == 0x08)
	{
		if(csr_x != 0) csr_x--;
	}
	/* Handles a tab by incrementing the cursor's x, but only
	 *  to a point that will make it divisible by 8 */
	else if(c == 0x09)
	{
		csr_x = (csr_x + 8) & ~(8 - 1);
	}
	/* Handles a 'Carriage Return', which simply brings the
	 *  cursor back to the margin */
	else if(c == '\r')
	{
		csr_x = 0;
	}
	/* We handle our newlines the way DOS and the BIOS do: we
	 *  treat it as if a 'CR' was also there, so we bring the
	 *  cursor to the margin and we increment the 'y' value */
	else if(c == '\n')
	{
		csr_x = 0;
		csr_y++;
	}
	/* Any character greater than and including a space, is a
	 *  printable character. The equation for finding the index
	 *  in a linear chunk of memory can be represented by:
	 *  Index = [(y * width) + x] */
	else if(c >= ' ')
	{
		where = textmemptr + (csr_y * 80 + csr_x);
		*where = c | att;	/* Character AND attributes: color */
		csr_x++;
	}
	
	/* If the cursor has reached the edge of the screen's width, we
	 *  insert a new line in there */
	if(csr_x >= 80)
	{
		csr_x = 0;
		csr_y++;
	}
	
	/* Scroll the screen if needed, and finally move the cursor */
	scroll();
	move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text)
{
	int i;
	
	for (i = 0; i < strlen(text); i++)
	{
		putch(text[i]);
	}
}