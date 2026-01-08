#include "../include/wav.h"
#include "../include/byte_util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WAV_HEADER_SIZE_IN_BYTES 44

Wav *init_wav(uint16_t audio_format, uint16_t channel_count,
              uint32_t sample_rate, uint16_t bits_per_sample,
              uint32_t length_in_seconds) {
  uint32_t sample_count = length_in_seconds * sample_rate;

  Wav *wav = malloc(sizeof(Wav));
  int8_t *sample_bytes =
      malloc((bits_per_sample / 8) * sample_count * channel_count);

  if (sample_bytes == NULL || wav == NULL) {
    printf("Could not allocate wav struct\n");
    free(sample_bytes);
    free(wav);
    return NULL;
  }

  wav->audio_format = audio_format;
  wav->channel_count = channel_count;
  wav->sample_rate = sample_rate;
  wav->bits_per_sample = bits_per_sample;
  wav->sample_count = sample_count;
  wav->sample_bytes = sample_bytes;

  return wav;
}

void free_wav(Wav *data) {
  free(data->sample_bytes);
  data->sample_bytes = NULL;
  free(data);
}

uint8_t write_wav_to_file(Wav *data, char *filename) {
  FILE *wav_file = fopen(filename, "wb");
  if (wav_file == NULL) {
    perror("Could not open wav file");
    return WRITE_WAV_COULD_NOT_OPEN_ERROR;
  }

  uint32_t byte_rate =
      (data->sample_rate * data->bits_per_sample * data->channel_count) / 8;

  uint16_t block_align = (data->bits_per_sample / 8) * data->channel_count;

  uint32_t sample_bytes_count =
      (data->bits_per_sample / 8) * data->sample_count * data->channel_count;

  uint32_t total_file_size = sample_bytes_count + WAV_HEADER_SIZE_IN_BYTES - 8;

  uint8_t wav_header[WAV_HEADER_SIZE_IN_BYTES] = {
      // RIFF Header
      0x52, 0x49, 0x46, 0x46,
      // File Size
      NTH_BYTE_OF(total_file_size, 0), NTH_BYTE_OF(total_file_size, 1),
      NTH_BYTE_OF(total_file_size, 2), NTH_BYTE_OF(total_file_size, 3),
      // WAVE Header
      0x57, 0x41, 0x56, 0x45,
      // FMT Header
      0x66, 0x6D, 0x74, 0x20,
      // Chunk Size (= 16)
      0x10, 0x00, 0x00, 0x00,
      // Audio Format
      NTH_BYTE_OF(data->audio_format, 0), NTH_BYTE_OF(data->audio_format, 1),
      // Channel Count
      NTH_BYTE_OF(data->channel_count, 0), NTH_BYTE_OF(data->channel_count, 1),
      // Sample Rate
      NTH_BYTE_OF(data->sample_rate, 0), NTH_BYTE_OF(data->sample_rate, 1),
      NTH_BYTE_OF(data->sample_rate, 2), NTH_BYTE_OF(data->sample_rate, 3),
      // Byte Rate
      NTH_BYTE_OF(byte_rate, 0), NTH_BYTE_OF(byte_rate, 1),
      NTH_BYTE_OF(byte_rate, 2), NTH_BYTE_OF(byte_rate, 3),
      // Block Align
      NTH_BYTE_OF(block_align, 0), NTH_BYTE_OF(block_align, 1),
      // BitsPerSample
      NTH_BYTE_OF(data->bits_per_sample, 0),
      NTH_BYTE_OF(data->bits_per_sample, 1),
      // data header
      0x64, 0x61, 0x74, 0x61,
      // data size
      NTH_BYTE_OF(sample_bytes_count, 0), NTH_BYTE_OF(sample_bytes_count, 1),
      NTH_BYTE_OF(sample_bytes_count, 2), NTH_BYTE_OF(sample_bytes_count, 3)};

  size_t written_header_bytes =
      fwrite(wav_header, 1, WAV_HEADER_SIZE_IN_BYTES, wav_file);
  size_t written_sample_bytes =
      fwrite(data->sample_bytes, 1, sample_bytes_count, wav_file);
  fclose(wav_file);

  size_t total_written_bytes = written_header_bytes + written_sample_bytes;
  size_t expected_written_bytes = WAV_HEADER_SIZE_IN_BYTES + sample_bytes_count;
  if (total_written_bytes != expected_written_bytes) {
    printf("Could not write every byte (%lu out of %lu)\n", total_written_bytes,
           expected_written_bytes);
    return WRITE_WAV_FILE_FAILED;
  }

  return WRITE_WAV_SUCCESS;
}
