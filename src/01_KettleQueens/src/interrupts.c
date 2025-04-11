#include "terminal.h"

// Inline function to send values to an I/O port
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void isr33_handler() {
    terminal_write("Interrupt 33 fired!\n");

    // Send End Of Interrupt (EOI) to PIC (for IRQ1 / interrupt 33)
    outb(0x20, 0x20);
}

void isr32_handler() {
    terminal_write("Interrupt 32 fired!\n");
}

void isr34_handler() {
    terminal_write("Interrupt 34 fired!\n");
}
