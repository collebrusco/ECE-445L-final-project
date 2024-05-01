#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

enum ToneGenerator {
    TONE_A = 0x01,
    TONE_B = 0x02,
    TONE_C = 0x04
};

void sound_init(enum ToneGenerator tones);

void tone_start(enum ToneGenerator tone, uint16_t frequency);
void tone_stop(enum ToneGenerator tone);
void tone_set_table(enum ToneGenerator tone, const uint16_t* table, uint32_t size);

void sound_stop();


#endif
