#include "libc/stdint.h"
#include "gdt.h"
#include "idt.h"
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
    gdt_init();         // Load GDT
    terminal_write("Installing IDT...\n");
    idt_install();      // Load IDT
    terminal_write("IDT installed, enabling interrupts...\n");

    __asm__ volatile ("sti");  // Enable hardware interrupts

    terminal_init();
    terminal_write("Hello World!\n");

    __asm__ volatile ("int $0x20"); // should trigger isr32_handler
    __asm__ volatile ("int $0x22"); // should trigger isr34_handler


    while (1) {
        __asm__ volatile ("hlt");  // Halt CPU until next interrupt
    }

    return 0;
}

