#ifndef MONITOR_H
#define MONITOR_H

#include "libc/stdint.h"
#include "libc/stddef.h"

void monitor_initialize() ;
void monitor_setcolor(uint8_t color);

void monitor_put(char c);
void monitor_clear();
void monitor_write(const char* data, size_t size);
void monitor_write_hex(uint32_t n);
void monitor_write_dec(uint32_t n);

#endif