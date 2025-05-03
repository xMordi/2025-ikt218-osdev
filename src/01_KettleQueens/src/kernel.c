#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "multiboot2.h"

#include "gdt/gdt.h"
#include "interrupt/idt.h"
#include "printing/terminal.h"
#include "interrupt/keyboard.h"
#include "interrupt/interrupts.h"

#include "kernel/memory.h"

extern uint32_t end;


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


// Added to see assignment 4 part 1 working
void print_ptr(void* ptr) {
    uintptr_t val = (uintptr_t)ptr;
    char buf[11] = "0x00000000";

    const char* hex = "0123456789ABCDEF";
    for (int i = 9; i >= 2; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }

    terminal_write(buf);
    terminal_write("\n");
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_init();
    terminal_init();
    idt_install();
    
    irq_install_handler(1, keyboard_handler);

    terminal_write("Hello World!\n");
    terminal_write("Testing divide by zero exception...\n");


    // assignment 4
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();

    void* some_memory = malloc(12345);
    terminal_write("some_memory: ");
    print_ptr(some_memory);

    void* memory2 = malloc(54321);
    terminal_write("memory2: ");
    print_ptr(memory2);

    void* memory3 = malloc(13331);
    terminal_write("memory3: ");
    print_ptr(memory3);

    // Commented out for the keyboard logger
    // __asm__ volatile ("int $0x0");  // Trigger divide by zero exception


    while (1) {
    __asm__ volatile ("hlt");  // Halt CPU until next interrupt
    }

    return 0;
}

