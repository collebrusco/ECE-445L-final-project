#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H
#include <stdint.h>

void engine_sound_init();

void engine_sound_start(int32_t rpm);
void engine_sound_up_rpm(int32_t rpm);
void engine_sound_stop();

#endif
