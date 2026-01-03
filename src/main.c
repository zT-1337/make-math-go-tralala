#include "../include/musical_notes.h"
#include "../include/wav.h"
#include <math.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846

#define AUDIO_FILENAME "sample.wav"
#define AUDIO_LENGTH_IN_SECONDS 10
#define AMPLITUDE INT16_MAX

uint8_t linear(uint8_t x) { return 3 * x; }

void math_to_tralala(Wav *audio_data, uint8_t (*do_math)(uint8_t)) {
  uint32_t sample_size_in_bytes =
      (audio_data->bits_per_sample / 8) * audio_data->channel_count;
  uint32_t second_size_in_bytes =
      sample_size_in_bytes * audio_data->sample_rate;

  for (uint8_t second = 0; second < AUDIO_LENGTH_IN_SECONDS; ++second) {
    double note_frequency = get_note_frequency(do_math(second));
    for (uint32_t sample = 0; sample < audio_data->sample_rate; ++sample) {
      int32_t sound = AMPLITUDE * sin((2 * M_PI * note_frequency * sample) /
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

int main() {
  Wav *audio_data =
      init_wav(PCM_AUDIO_FORMAT, STEREO_CHANNEL, CD_SAMPLE_RATE,
               DEFAULT_SAMPLE_SIZE_IN_BITS, AUDIO_LENGTH_IN_SECONDS);

  math_to_tralala(audio_data, linear);

  uint8_t write_result = write_wav_to_file(audio_data, AUDIO_FILENAME);

  free_wav(audio_data);
  return write_result;
}
