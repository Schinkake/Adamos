
// TODO Change Types to those of types.h

#ifndef __ISRS_H
#define __ISRS_H

#include <common/types.h>

/* This defines what the stack looks like
 * after an ISR was running */
struct regs
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;    
};

/* ISRS.C */
void isr_install();
void _fault_handler(struct regs *r);

#endif