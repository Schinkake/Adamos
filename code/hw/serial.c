#include <hw/serial.h>
#include <hw/port.h>

void init_serial(uint16_t port) {

	port8BitWrite(port + 1, 0x00);    // Disable all interrupts
	port8BitWrite(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	port8BitWrite(port + 0, 0x0C);    // Set divisor to 3 (lo byte) 38400 baud
	port8BitWrite(port + 1, 0x00);    //                  (hi byte)
	port8BitWrite(port + 3, 0x03);    // 8 bits, no parity, one stop bit
	port8BitWrite(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	port8BitWrite(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

uint8_t serial_received(uint16_t port) {
	
	return port8BitRead(port + 5) & 1;
}

uint8_t read_serial(uint16_t port) {
	
	while (serial_received(port) == 0);
	return port8BitRead(port);
}

uint8_t transmit_empty(uint16_t port) {
	
	return port8BitRead(port + 5) & 0x20;
}

void write_serial(uint16_t port, uint8_t data) {
	
	while (transmit_empty(port) == 0);
	port8BitWrite(port, data);
}