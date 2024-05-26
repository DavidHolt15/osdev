#ifndef COMMON_H
#define COMMON_H

#include "libc/stdint.h"

void writePort8(uint16_t port, uint8_t value);
uint8_t readPort8(uint16_t port);
uint16_t readPort16(uint16_t port);

#endif