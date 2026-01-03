#include "../include/musical_notes.h"
#include "../include/tralala.h"
#include "../include/wav.h"
#include <stdint.h>

#define AUDIO_FILENAME "sample.wav"

int main() {
  Wav *audio_data =
      init_wav(PCM_AUDIO_FORMAT, STEREO_CHANNEL, CD_SAMPLE_RATE,
               DEFAULT_SAMPLE_SIZE_IN_BITS, AUDIO_LENGTH_IN_SECONDS);

  math_to_tralala(audio_data, linear);

  uint8_t write_result = write_wav_to_file(audio_data, AUDIO_FILENAME);

  free_wav(audio_data);
  return write_result;
}
