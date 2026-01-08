#include "../include/musical_notes.h"
#include "../include/options.h"
#include "../include/tralala.h"
#include "../include/wav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATHERS_COUNT 5
#define FAILED_TO_INIT_WAV_STRUCT -1

uint8_t generate_wav_files(Options *opts) {
  char filename_buffer[128];
  uint8_t (*mathers[MATHERS_COUNT])(uint8_t) = {linear, quadratic, sinus,
                                                logarithmic, randomized};

  for (int i = 0; i < MATHERS_COUNT; ++i) {
    Wav *audio_data =
        init_wav(PCM_AUDIO_FORMAT, opts->channel_count, opts->sample_rate,
                 opts->bits_per_sample, AUDIO_LENGTH_IN_SECONDS);
    if (audio_data == NULL) {
      return FAILED_TO_INIT_WAV_STRUCT;
    }

    math_to_tralala(audio_data, mathers[i]);

    snprintf(filename_buffer, sizeof(filename_buffer), "sample_%d.wav", i);
    uint8_t write_result = write_wav_to_file(audio_data, filename_buffer);
    free_wav(audio_data);

    if (write_result != WRITE_WAV_SUCCESS) {
      return write_result;
    }

    printf("Generated %s\n", filename_buffer);
  }

  return 0;
}

int main(int argc, char **argv) {
  Options opts = parse_options(argc, argv);

  srand(time(NULL));
  uint8_t result = generate_wav_files(&opts);

  return result;
}
