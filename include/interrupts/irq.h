#ifndef __IRQ_H
#define __IRQ_H

#include <interrupts/isr.h>

void irq_install_handler(int irq, void (*handler)(struct regs* r));
void irq_uninstall_handler(int irq);
void irq_remap();
void irq_install();
void _irq_handler(struct regs *r);

#endif