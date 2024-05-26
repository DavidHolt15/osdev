#include "keyboard.h"
#include "screen.h"
#include "common.h"

#define KEYBOARD_BUFFER_SIZE 256

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint16_t keyboard_buffer_index = 0;

static const char scancode_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, ' ', 0, ' ', 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler(registers_t regs, void* ctx) {
    uint8_t scancode = readPort8(0x60);
    char ascii = scancode_to_ascii[scancode];

    if (ascii != 0 && keyboard_buffer_index < KEYBOARD_BUFFER_SIZE) {
        keyboard_buffer[keyboard_buffer_index++] = ascii;
        screen_put(ascii);
    }
}

void init_keyboard() {
    register_irq_handler(IRQ1, keyboard_handler, NULL);
}