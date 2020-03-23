#ifndef __SERIAL_H
#define __SERIAL_H

#include <common/types.h>

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

void init_serial(uint16_t port);
uint8_t serial_received(uint16_t port);
uint8_t read_serial(uint16_t port);
uint8_t transmit_empty(uint16_t port);
void write_serial(uint16_t port, uint8_t data);

#endif