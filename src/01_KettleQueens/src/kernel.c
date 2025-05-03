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
#include "kernel/pit.h"

#include "song/frequencies.h"
#include "song/song.h"

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

extern void play_song_impl(Song* song);
extern SongPlayer* create_song_player();
extern Note music_1[];
extern uint32_t music_1_length;

void play_music() {
    Song songs[] = {
        { music_1, music_1_length }
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    terminal_write("🎵 Starting music playback...\n");

    for (uint32_t i = 0; i < n_songs; i++) {
        terminal_write("Playing song...\n");
        player->play_song(&songs[i]);
        terminal_write("Finished song.\n");
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_init();
    idt_install();
    terminal_init();
    irq_install_handler(1, keyboard_handler);

    terminal_write("Hello World!\n");
    // terminal_write("Testing divide by zero exception...\n");
    // __asm__ volatile ("int $0x0");  // Trigger divide by zero exception

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


    // Initialize the PIT (Programmable Interval Timer)
    init_pit();
    terminal_write("PIT initialized.\n");
    terminal_write("Sleeping for 5 seconds...\n");
    sleep_interrupt(500);  // Sleep for 5 seconds
    terminal_write("Awake!\n");

    terminal_write("Sleeping for 5 seconds (busy wait)...\n");
    sleep_busy(500);  // Sleep for 5 seconds using busy wait   
    terminal_write("Awake from busy wait!\n");

    play_music();


    while (1) {
    __asm__ volatile ("hlt");  // Halt CPU until next interrupt
    }

    return 0;
}

