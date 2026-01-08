#include "../include/tralala.h"
#include "../include/musical_notes.h"
#include "../include/wav.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

#define OFFSET 16

uint8_t linear(uint8_t x) { return x + OFFSET; }

uint8_t quadratic(uint8_t x) { return x * x * 0.25 + OFFSET; }

uint8_t logarithmic(uint8_t x) { return log10(x * 2) + OFFSET; }

uint8_t sinus(uint8_t x) { return sin(x * 1.5) * 8 + OFFSET; }

uint8_t randomized(uint8_t x __attribute__((unused))) {
  return rand() % (NOTE_COUNT + 1);
}

void math_to_tralala(Wav *audio_data, uint8_t (*do_math)(uint8_t)) {
  uint32_t amplitude;
  if (audio_data->bits_per_sample == LOWEST_SAMPLE_SIZE_IN_BITS) {
    amplitude = INT8_MAX;
  } else if (audio_data->bits_per_sample == DEFAULT_SAMPLE_SIZE_IN_BITS) {
    amplitude = INT16_MAX;
  } else {
    amplitude = INT32_MAX;
  }

  uint32_t sample_size_in_bytes =
      (audio_data->bits_per_sample / 8) * audio_data->channel_count;

  uint32_t second_size_in_bytes =
      sample_size_in_bytes * audio_data->sample_rate;

  for (uint8_t second = 0; second < AUDIO_LENGTH_IN_SECONDS; ++second) {
    double note_frequency = get_note_frequency(do_math(second));
    for (uint32_t sample = 0; sample < audio_data->sample_rate; ++sample) {
      int32_t sound = amplitude * sin((2 * M_PI * note_frequency * sample) /
                                      CD_SAMPLE_RATE);

      uint32_t sample_start_position =
          second * second_size_in_bytes + sample * sample_size_in_bytes;

      for (uint16_t channel = 0; channel < audio_data->channel_count;
           ++channel) {
        uint16_t channel_offset = channel * (audio_data->bits_per_sample / 8);

        for (uint16_t sample_byte = 0;
             sample_byte < (audio_data->bits_per_sample / 8); ++sample_byte) {
          audio_data->sample_bytes[sample_start_position + channel_offset +
                                   sample_byte] =
              (sound >> (8 * sample_byte)) & 0xFF;
        }
      }
    }
  }
}
