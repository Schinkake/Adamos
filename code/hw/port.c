#include <hw/port.h>

uint8_t port8BitRead(uint16_t port) {
	
	uint8_t result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void port8BitWrite(uint16_t port, uint8_t data) {
	
	__asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}


uint16_t port16BitRead(uint16_t port) {
	
	uint16_t result;
	__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void port16BitWrite(uint16_t port, uint16_t data) {
	
	__asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
}

uint32_t port32BitRead(uint16_t port) {
	
	uint32_t result;
	__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void port32BitWrite(uint16_t port, uint32_t data) {
	
	__asm__ volatile("outl %0, %1" : : "a"(data), "Nd" (port));
}