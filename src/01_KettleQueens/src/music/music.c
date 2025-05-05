#include "kernel/pit.h"
#include "music/music.h"
#include "util.h"
#include "libc/stdbool.h"
#include "kernel/memory.h"

void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    if (tmp != (tmp | 3))
    {
        outb(PC_SPEAKER_PORT, tmp | 3);
    }
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT & 0xFC);
    outb(PC_SPEAKER_PORT, tmp); // Disable the speaker
}

void play_sound(uint32_t frequency) {
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    outb(PIT_CMD_PORT, 0xB6); // Set the command byte
    outb(PIT_CHANNEL2_PORT,(uint8_t) (divisor)); // Set the low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t) (divisor >> 8)); // Set the high byte

    enable_speaker(); // Enable the speaker
}

void stop_sound() {
    disable_speaker(); // Disable the speaker
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
    .notes = starwars_theme,
    .length = sizeof(starwars_theme) / sizeof(Note)
};

void test_speaker() {
    play_sound(440); // Set a valid frequency
    enable_speaker(); // Enable the speaker
    sleep_interrupt(2000); // Hold the tone for 2 seconds
    disable_speaker(); // Disable the speaker
    stop_sound(); // Stop the PIT signal
}

