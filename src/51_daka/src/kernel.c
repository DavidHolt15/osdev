#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/system.h"

#include "multiboot2.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "keyboard.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag first;
};

int kernel_main();
extern uint32_t end;

int main(uint32_t magic, struct multiboot_info mb_info_addr) {
    screen_initialize();
    init_gdt();
    printf("Hello, World!\n");

    init_idt();
    init_irq();
    init_keyboard();
    asm volatile ("sti");

    return kernel_main();
}