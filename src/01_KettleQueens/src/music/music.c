#include "kernel/pit.h"
#include "music/music.h"
#include "util.h"
#include "libc/stdbool.h"
#include "kernel/memory.h"

void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 0x03); // Enable the speaker
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); // Disable the speaker
}

void play_sound(uint32_t frequency) {
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    outb(PIT_CMD_PORT, 0xB6); // Set the command byte
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF); // Set the low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // Set the high byte
    enable_speaker();
}

void stop_sound() {
    disable_speaker();
    outb(PIT_CMD_PORT, 0xB6); // Reset the command byte
    outb(PIT_CHANNEL0_PORT, 0x00); // Stop the sound
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player == NULL) {
        return NULL; // Memory allocation failed
    }
    player->play_song = play_song;
    return player;
}

void play_song(Song* song) {
    for (uint32_t i = 0; i < song->length; i++) {
        Note note = song->notes[i];
        if (note.frequency == R) {
            stop_sound();
            sleep_interrupt(note.duration);
        } else {
            play_sound(note.frequency);
            sleep_interrupt(note.duration);
            stop_sound();
        }
    }
}

Song starwars_song = {
    .notes = starwars,
    .length = sizeof(starwars) / sizeof(Note)
};