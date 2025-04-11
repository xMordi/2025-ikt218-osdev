#include "../include/idt.h"
#include "libc/stdint.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Optional: read byte from port (can be used later if needed)
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// From isr.s or your ISR assembly file
extern void isr33();
extern void isr32();
extern void isr34();

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_reg;

void idt_set_gate(int n, uint32_t handler) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].sel       = 0x08;     // Kernel code segment selector
    idt[n].always0   = 0;
    idt[n].flags     = 0x8E;     // Present, Ring 0, 32-bit interrupt gate
}

// 🔧 PIC remap function
void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // Master PIC vector offset = 0x20 (32)
    outb(0xA1, 0x28); // Slave PIC vector offset  = 0x28 (40)
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Unmask all IRQs (or just IRQ1 if preferred)
    outb(0x21, 0xFD); // 11111101 - allow IRQ1 (keyboard)
    outb(0xA1, 0xFF); // Mask all on slave PIC
}

void idt_install() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0);
    }

    pic_remap();  // 🔥 Remap IRQs 0–15 to 32–47

    idt_set_gate(33, (uint32_t)isr33);  // IRQ1 mapped to 0x21 = 33
    idt_set_gate(32, (uint32_t)isr32);
    idt_set_gate(34, (uint32_t)isr34);


    idt_flush((uint32_t)&idt_reg);
}
