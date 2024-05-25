#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/system.h"

#include "multiboot2.h"
#include "monitor.h"
#include "gdt.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();
extern uint32_t end;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    monitor_initialize();
    init_gdt();
    printf("Hello, World!\n");

    return kernel_main();
}