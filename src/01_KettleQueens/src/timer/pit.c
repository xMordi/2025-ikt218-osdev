#include "kernel/pit.h"
#include "util.h"


void init_pit() {
    // Set the PIT to mode 2 (rate generator) with a divisor of 0x4E20
    outb(PIT_CMD_PORT, 0x34); // Command byte: 00110100
    outb(PIT_CHANNEL0_PORT, (uint8_t)(PIT_DEFAULT_DIVISOR & 0xFF)); // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((PIT_DEFAULT_DIVISOR >> 8) & 0xFF)); // High byte

    // Unmask the IRQ0 line on the PIC
    outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~0x01);
}

void sleep_interrupt(uint32_t milliseconds) {
    // Calculate the number of ticks needed for the specified milliseconds
    uint32_t ticks = milliseconds * TICKS_PER_MS;

    // Set up a timer interrupt handler to wake up the process
    // This is a placeholder; actual implementation would require setting up an interrupt handler
    // and using a condition variable or semaphore to block the process until the timer expires.
    // For now, we will just busy wait.
    for (uint32_t i = 0; i < ticks; i++) {
        // Wait for the PIT interrupt
        while (!(inb(PIC1_CMD_PORT) & 0x01));
        outb(PIC1_CMD_PORT, PIC_EOI); // Send EOI to PIC
    }
}

void sleep_busy(uint32_t milliseconds) {
    // Calculate the number of ticks needed for the specified milliseconds
    uint32_t ticks = milliseconds * TICKS_PER_MS;

    // Busy wait for the specified number of ticks
    for (uint32_t i = 0; i < ticks; i++) {
        // Wait for the PIT interrupt
        while (!(inb(PIC1_CMD_PORT) & 0x01));
        outb(PIC1_CMD_PORT, PIC_EOI); // Send EOI to PIC
    }
}