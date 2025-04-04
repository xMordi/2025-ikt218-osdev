#include "terminal.h"
#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F

static uint16_t* video_memory = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

void terminal_init() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_row = 0;
    cursor_col = 0;
}

void terminal_write(const char* str) {
    while (*str) {
        char c = *str++;
        if (c == '\n') {
            cursor_row++;
            cursor_col = 0;
            continue;
        }

        video_memory[cursor_row * 80 + cursor_col] = (WHITE_ON_BLACK << 8) | c;
        cursor_col++;
        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }
    }
}
