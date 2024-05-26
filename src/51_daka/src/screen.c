#include "screen.h"
#include "libc/system.h"
#include "common.h"

enum vga_color {
    VGA_COLOR_LIGHT_GREY = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_BLACK = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_WHITE = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_LIGHT_RED = 15,
};

static const size_t SCREEN_WIDTH = 80;
static const size_t SCREEN_HEIGHT = 25;

uint16_t *video_memory = (uint16_t *)0xB8000;
size_t screen_row;
size_t screen_column;
uint8_t screen_color;
uint16_t* screen_buffer;

static void scroll_screen() {
    uint8_t attributeByte = (7 << 4) | (12 & 0x0F);
    uint16_t blank = 0x20 | (attributeByte << 8);
    if (screen_row >= SCREEN_HEIGHT) {
        for (size_t i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i++) {
            screen_buffer[i] = screen_buffer[i + SCREEN_WIDTH];
        }
        for (size_t i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
            screen_buffer[i] = blank;
        }
        screen_row = SCREEN_HEIGHT - 1;
    }
}

static void update_cursor() {
    uint16_t pos = screen_row * SCREEN_WIDTH + screen_column;
    writePort8(0x3D4, 0x0F);
    writePort8(0x3D5, (uint8_t)(pos & 0xFF));
    writePort8(0x3D4, 0x0E);
    writePort8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static inline uint8_t get_vga_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t get_vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void screen_initialize(void) {
    screen_row = 0;
    screen_column = 0;
    screen_color = get_vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    screen_buffer = video_memory;
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            const size_t index = y * SCREEN_WIDTH + x;
            screen_buffer[index] = get_vga_entry(' ', screen_color);
        }
    }
}

void screen_setcolor(uint8_t color) {
    screen_color = color;
}

void screen_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * SCREEN_WIDTH + x;
    screen_buffer[index] = get_vga_entry(c, color);
}

void _screen_put(char c) {
    switch (c) {
        case '\n':
            screen_column = 0;
            screen_row++;
            scroll_screen();
            return;
        default:
            break;
    }

    screen_putentryat(c, screen_color, screen_column, screen_row);
    if (++screen_column == SCREEN_WIDTH) {
        screen_column = 0;
        if (++screen_row == SCREEN_HEIGHT)
            screen_row = 0;
    }
}

void screen_put(char c) {
    _screen_put(c);
    scroll_screen();
    update_cursor();
}

void screen_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        _screen_put(data[i]);

    scroll_screen();
    update_cursor();
}

void screen_writestring(const char* data) {
    screen_write(data, strlen(data));
}

void screen_clear() {
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        screen_buffer[i] = blank;
    }

    screen_row = 0;
    screen_column = 0;
    update_cursor();
}

void screen_write_hex(uint32_t n) {
    char* item = "0x";
    screen_write(item, strlen(item));

    char noZeroes = 1;
    int32_t tmp;

    for (int i = 28; i >= 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes) continue;
        noZeroes = 0;
        screen_put(tmp >= 0xA ? tmp - 0xA + 'a' : tmp + '0');
    }
}

void screen_write_dec(uint32_t n) {
    if (n == 0) {
        screen_put('0');
        return;
    }

    char buffer[32];
    int i = 0;
    while (n > 0) {
        buffer[i++] = '0' + n % 10;
        n /= 10;
    }
    buffer[i] = '\0';

    char reversed[32];
    for (int j = 0; j < i; j++) {
        reversed[j] = buffer[i - j - 1];
    }
    reversed[i] = '\0';

    screen_write(reversed, strlen(reversed));
}

void screen_clear_line(size_t line) {
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);
    for (size_t x = 0; x < SCREEN_WIDTH; x++) {
        screen_buffer[line * SCREEN_WIDTH + x] = blank;
    }
}

void screen_set_cursor(size_t row, size_t column) {
    screen_row = row;
    screen_column = column;
    update_cursor();
}
