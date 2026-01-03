#include "../include/musical_notes.h"
#include "../include/tralala.h"
#include "../include/wav.h"
#include <stdint.h>
#include <stdio.h>

#define MATHERS_COUNT 1

int main() {
  char filename_buffer[128];
  uint8_t (*mathers[1])(uint8_t) = {linear};
  for (int i = 0; i < MATHERS_COUNT; ++i) {
    Wav *audio_data =
        init_wav(PCM_AUDIO_FORMAT, STEREO_CHANNEL, CD_SAMPLE_RATE,
                 DEFAULT_SAMPLE_SIZE_IN_BITS, AUDIO_LENGTH_IN_SECONDS);

    math_to_tralala(audio_data, mathers[i]);

    snprintf(filename_buffer, sizeof(filename_buffer), "sample_%d.wav", i);
    uint8_t write_result = write_wav_to_file(audio_data, filename_buffer);
    free_wav(audio_data);

    if (write_result != 0) {
      return write_result;
    }
  }

  return 0;
}
