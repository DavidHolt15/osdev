#include "interrupts.h"
#include "screen.h"

void isr_handler(registers_t* regs, void* ctx) {
    if (regs->int_no < 32) {
        screen_write("Received interrupt: ", 19);
        screen_write_dec(regs->int_no);
        screen_put('\n');
    }
}