#include "wav.h"
#include <stdint.h>

#ifndef ZT_TRALALA_H
#define ZT_TRALALA_H

#define AUDIO_LENGTH_IN_SECONDS 10

void math_to_tralala(Wav *data, uint8_t (*do_math)(uint8_t x));
uint8_t linear(uint8_t x);
uint8_t quadratic(uint8_t x);
uint8_t logarithmic(uint8_t x);
uint8_t sinus(uint8_t x);
uint8_t randomized(uint8_t x);

#endif
