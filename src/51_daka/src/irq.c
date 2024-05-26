#include "interrupts.h"
#include "common.h"

void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = NULL;
        irq_handlers[i].data = NULL;
    }
}

void register_irq_handler(int irq, isr_t handler, void* ctx) {
    irq_handlers[irq].handler = handler;
    irq_handlers[irq].data = ctx;
}

void irq_handler(registers_t* regs, void* ctx) {
    if (regs->int_no >= 40) {
        writePort8(0xA0, 0x20);
    }
    writePort8(0x20, 0x20);

    if (irq_handlers[regs->int_no - 32].handler != NULL) {
        irq_handlers[regs->int_no - 32].handler(regs, irq_handlers[regs->int_no - 32].data);
    }
}