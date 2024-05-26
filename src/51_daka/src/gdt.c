#include "gdt.h"

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdt_descriptor;

extern void gdt_flush(uint32_t gdt_ptr);

void init_gdt() {
    gdt_descriptor.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;
    gdt_descriptor.base = (uint32_t)&gdt;
    setGdtEntry(0, 0, 0, 0, 0);
    setGdtEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    setGdtEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    setGdtEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    setGdtEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdt_descriptor);
}

void setGdtEntry(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low    = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high   = (base >> 24) & 0xFF;

    gdt[index].limit_low   = (limit & 0xFFFF);
    gdt[index].granularity = (limit >> 16) & 0x0F;

    gdt[index].granularity |= granularity & 0xF0;
    gdt[index].access      = access;
}
