#ifndef __PORT_H
#define __PORT_H

#include <common/types.h>

uint8_t 	port8BitRead(uint16_t port);
void 		port8BitWrite(uint16_t port, uint8_t data);
uint16_t 	port16BitRead(uint16_t port);
void 		port16BitWrite(uint16_t port, uint16_t data);
uint32_t	port32BitRead(uint16_t port);
void 		port32BitWrite(uint16_t port, uint32_t data);

#endif