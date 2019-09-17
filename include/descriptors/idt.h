
// TODO Change Types to those of types.h

#ifndef __IDT_H
#define __IDT_H

#include <common/types.h>

/* IDT.C */
void _idt_load();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

#endif