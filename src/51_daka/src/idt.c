#include "idt.h"
#include "interrupts.h"
#include "common.h"

extern void idt_flush(uint32_t);

void init_idt() {
    idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t) &idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].base_low = 0x0000;
        idt[i].base_high = 0x0000;
        idt[i].selector = 0x08;
        idt[i].zero = 0x00;
        idt[i].flags = 0x8E;
        int_handlers[i].handler = NULL;
    }

    init_interrupts();
    idt_flush((uint32_t)&idt_ptr);
}

void idt_load() {
    asm volatile("lidt %0" : : "m" (idt_ptr));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags | 0x60;
}

void init_interrupts() {
    writePort8(0x20, 0x11);
    writePort8(0xA0, 0x11);
    writePort8(0x21, 0x20);
    writePort8(0xA1, 0x28);
    writePort8(0x21, 0x04);
    writePort8(0xA1, 0x02);
    writePort8(0x21, 0x01);
    writePort8(0xA1, 0x01);
    writePort8(0x21, 0x0);
    writePort8(0xA1, 0x0);

    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    // Continue setting gates for other ISRs and IRQs as needed
}