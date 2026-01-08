#include <stdint.h>

#ifndef ZT_WAV_H
#define ZT_WAV_H

// Audio Format Definitions
#define PCM_AUDIO_FORMAT 1

// Channel Definitions
#define MONO_CHANNEL 1
#define STEREO_CHANNEL 2

// Sample Rate Definitions
#define CD_SAMPLE_RATE 44100
#define DVD_SAMPLE_RATE 48000
#define HIGH_END_SAMPLE_RATE 192000

// Sample Size Definitions
#define LOWEST_SAMPLE_SIZE_IN_BITS 8
#define DEFAULT_SAMPLE_SIZE_IN_BITS 16
#define HIGHEST_SAMPLE_SIZE_IN_BITS 32

// Write to file results
#define WRITE_WAV_SUCCESS 0
#define WRITE_WAV_COULD_NOT_OPEN_ERROR 1
#define WRITE_WAV_FILE_FAILED 2

typedef struct Wav {
  uint16_t audio_format;
  uint16_t channel_count;
  uint32_t sample_rate;
  uint16_t bits_per_sample;
  uint32_t sample_count;
  int8_t *sample_bytes;
} Wav;

Wav *init_wav(uint16_t audio_format, uint16_t channel_count,
              uint32_t sample_rate, uint16_t bits_per_sample,
              uint32_t length_in_seconds);

void free_wav(Wav *data);

uint8_t write_wav_to_file(Wav *data, char *filename);

#endif
