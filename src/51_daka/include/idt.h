#ifndef IDT_H
#define IDT_H

#define IDT_ENTRIES 256

#include "libc/system.h"

struct idt_entry_t {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();

void idt_load();

static struct idt_entry_t idt[IDT_ENTRIES];
static struct idt_ptr_t idt_ptr;

#endif 