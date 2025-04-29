#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt/gdt.h"
#include "interrupt/idt.h"
#include "printing/terminal.h"
#include "interrupt/keyboard.h"
#include "interrupt/interrupts.h"


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_init();
    idt_install();
    terminal_init();
    irq_install_handler(1, keyboard_handler);

    terminal_write("Hello World!\n");
    // terminal_write("Testing divide by zero exception...\n");

    // Commented out for the keyboard logger
    // __asm__ volatile ("int $0x0");  // Trigger divide by zero exception


    while (1) {
    __asm__ volatile ("hlt");  // Halt CPU until next interrupt
    }

    return 0;
}

