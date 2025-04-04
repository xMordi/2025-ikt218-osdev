#include "libc/stdint.h"
#include "../include/gdt.h"
//#include "../include/terminal.h"
#include "terminal.h"

void terminal_init();
void terminal_write(const char* str);

void vga_print(const char* str) {
    volatile char* video = (volatile char*) 0xB8000;
    while (*str) {
        *video++ = *str++;
        *video++ = 0x07;
    }
}

int main() {
    gdt_init();
    terminal_init();
    terminal_write("Hello World!");

    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}

