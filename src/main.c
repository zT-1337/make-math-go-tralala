#include "../include/musical_notes.h"
#include "../include/options.h"
#include "../include/tralala.h"
#include "../include/wav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATHERS_COUNT 5

uint8_t generate_wav_files(Options *opts) {
  char filename_buffer[128];
  uint8_t (*mathers[MATHERS_COUNT])(uint8_t) = {linear, quadratic, sinus,
                                                logarithmic, randomized};

  for (int i = 0; i < MATHERS_COUNT; ++i) {
    Wav *audio_data =
        init_wav(PCM_AUDIO_FORMAT, opts->channel_count, opts->sample_rate,
                 opts->bits_per_sample, AUDIO_LENGTH_IN_SECONDS);

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

int main(int argc, char **argv) {
  Options opts = parse_options(argc, argv);

  srand(time(NULL));
  uint8_t result = generate_wav_files(&opts);

  return result;
}
