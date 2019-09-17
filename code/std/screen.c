#include <std/memory.h>
#include <std/screen.h>
#include <hw/port.h>

/* These define our textpointer, our background and foreground
 *  colors (attributes), and x and y cursor coordinates */
extern unsigned short *textmemptr;
extern int attrib;
extern int csr_x, csr_y;

/* Scrolls the screen */
void scroll(void)
{
	unsigned blank, temp;
	
	/* A blank is defined as a space... we need to give it
	 *  backcolor too */
	blank = 0x20 | (attrib << 8);
	
	/* Row 25 is the end, this means we need to scroll up */
	if(csr_y >= 25)
	{
		/* Move the current text chunk that makes up the screen
		 *  back in the buffer by a line */
		temp = csr_y - 25 + 1;
		memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);
		
		/* Finally, we set the chunk of memory that occupies
		 *  the last line of text to our 'blank' character */
		memsetw (textmemptr + (25 - temp) * 80, blank, 80);
		csr_y = 25 - 1;
	}
}

/* Updates the hardware cursor: the little blinking line
 *  on the screen under the last character pressed! */
void move_csr(void)
{
	unsigned temp;
	
	/* The equation for finding the index in a linear
	 *  chunk of memory can be represented by:
	 *  Index = [(y * width) + x] */
	temp = csr_y * 80 + csr_x;
	
	/* This sends a command to indicies 14 and 15 in the
	 *  CRT Control Register of the VGA controller. These
	 *  are the high and low bytes of the index that show
	 *  where the hardware cursor is to be 'blinking'. To
	 *  learn more, you should look up some VGA specific
	 *  programming documents. A great start to graphics:
	 *  http://www.brackeen.com/home/vga */
	port8BitWrite(0x3D4, 14);
	port8BitWrite(0x3D5, temp >> 8);
	port8BitWrite(0x3D4, 15);
	port8BitWrite(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
	unsigned blank;
	int i;
	
	/* Again, we need the 'short' that will be used to
	 *  represent a space with color */
	blank = 0x20 | (attrib << 8);
	
	/* Sets the entire screen to spaces in our current
	 *  color */
	for(i = 0; i < 25; i++)
		memsetw (textmemptr + i * 80, blank, 80);
	
	/* Update out virtual cursor, and then move the
	 *  hardware cursor */
	csr_x = 0;
	csr_y = 0;
	move_csr();
}