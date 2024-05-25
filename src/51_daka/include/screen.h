#ifndef SCREEN_H
#define SCREEN_H

#include "libc/stdint.h"
#include "libc/stddef.h"

void screen_initialize();
void screen_setcolor(uint8_t color);

void screen_put(char c);
void screen_clear();
void screen_write(const char* data, size_t size);
void screen_write_hex(uint32_t n);
void screen_write_dec(uint32_t n);

#endif