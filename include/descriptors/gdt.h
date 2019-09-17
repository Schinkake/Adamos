
// TODO Change Types to those of types.h

#ifndef __GDT_H
#define __GDT_H

#include <common/types.h>

/* GDT.C */
void _gdt_flush();
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

#endif