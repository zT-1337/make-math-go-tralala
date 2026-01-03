#include "../include/wav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WAV_HEADER_SIZE_IN_BYTES 44
#define FILE_SIZE_OFFSET 4
#define AUDIO_FORMAT_OFFSET 20
#define CHANNEL_COUNT_OFFSET 22
#define SAMPLE_RATE_OFFSET 24
#define BYTE_RATE_OFFSET 28
#define BLOCK_ALIGN_OFFSET 32
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_SIZE_OFFSET 40

Wav *init_wav(uint16_t audio_format, uint16_t channel_count,
              uint32_t sample_rate, uint16_t bits_per_sample,
              uint32_t length_in_seconds) {
  Wav *wav = malloc(sizeof(Wav));
  if (wav == NULL) {
    printf("Could not allocate wav\n");
    return NULL;
  }

  wav->audio_format = audio_format;
  wav->channel_count = channel_count;
  wav->sample_rate = sample_rate;
  wav->bits_per_sample = bits_per_sample;
  wav->sample_count = length_in_seconds * sample_rate;

  wav->sample_bytes =
      malloc((bits_per_sample / 8) * wav->sample_count * channel_count);
  if (wav->sample_bytes == NULL) {
    printf("Could not allocate sample bytes\n");
    free(wav);
    return NULL;
  }

  return wav;
}

void free_wav(Wav *data) {
  free(data->sample_bytes);
  data->sample_bytes = NULL;
  free(data);
}

uint8_t write_wav_to_file(Wav *data, char *filename) {
  FILE *wav_file = fopen(filename, "w");
  if (wav_file == NULL) {
    perror("Could not open wav file");
    return WRITE_WAV_COULD_NOT_OPEN_ERROR;
  }

  uint8_t wav_header[WAV_HEADER_SIZE_IN_BYTES] = {
      // RIFF Header
      0x52, 0x49, 0x46, 0x46,
      // File Size (Placeholder)
      0x00, 0x00, 0x00, 0x00,
      // WAVE Header
      0x57, 0x41, 0x56, 0x45,
      // FMT Header
      0x66, 0x6D, 0x74, 0x20,
      // Chunk Size (= 16)
      0x10, 0x00, 0x00, 0x00,
      // Audio Format & Channel Count (Placeholder)
      0x00, 0x00, 0x00, 0x00,
      // Sample Rate (Placeholder)
      0x00, 0x00, 0x00, 0x00,
      // Byte Rate (Placeholder)
      0x00, 0x00, 0x00, 0x00,
      // Block Align & BitsPerSample (Placeholder)
      0x00, 0x00, 0x00, 0x00,
      // data header
      0x64, 0x61, 0x74, 0x61,
      // data size (Placeholder)
      0x00, 0x00, 0x00, 0x00};

  // Replace audio format placeholder
  wav_header[AUDIO_FORMAT_OFFSET] = data->audio_format & 0xFF;
  wav_header[AUDIO_FORMAT_OFFSET + 1] = (data->audio_format >> 8) & 0xFF;

  // Replace channel count placeholder
  wav_header[CHANNEL_COUNT_OFFSET] = data->channel_count & 0xFF;
  wav_header[CHANNEL_COUNT_OFFSET + 1] = (data->channel_count >> 8) & 0xFF;

  // Replace sample rate placeholder
  wav_header[SAMPLE_RATE_OFFSET] = data->sample_rate & 0xFF;
  wav_header[SAMPLE_RATE_OFFSET + 1] = (data->sample_rate >> 8) & 0xFF;
  wav_header[SAMPLE_RATE_OFFSET + 2] = (data->sample_rate >> 16) & 0xFF;
  wav_header[SAMPLE_RATE_OFFSET + 3] = (data->sample_rate >> 24) & 0xFF;

  // Replace byte rate placeholder
  uint32_t byte_rate =
      (data->sample_rate * data->bits_per_sample * data->channel_count) / 8;
  wav_header[BYTE_RATE_OFFSET] = byte_rate & 0xFF;
  wav_header[BYTE_RATE_OFFSET + 1] = (byte_rate >> 8) & 0xFF;
  wav_header[BYTE_RATE_OFFSET + 2] = (byte_rate >> 16) & 0xFF;
  wav_header[BYTE_RATE_OFFSET + 3] = (byte_rate >> 24) & 0xFF;

  // Replace block align placeholder
  uint16_t block_align = (data->bits_per_sample / 8) * data->channel_count;
  wav_header[BLOCK_ALIGN_OFFSET] = block_align & 0xFF;
  wav_header[BLOCK_ALIGN_OFFSET + 1] = (block_align >> 8) & 0xFF;

  // Replace bits per sample placeholder
  wav_header[BITS_PER_SAMPLE_OFFSET] = data->bits_per_sample & 0xFF;
  wav_header[BITS_PER_SAMPLE_OFFSET + 1] = (data->bits_per_sample >> 8) & 0xFF;

  // Replace data size placeholder
  uint32_t sample_bytes_count =
      (data->bits_per_sample / 8) * data->sample_count * data->channel_count;
  wav_header[DATA_SIZE_OFFSET] = sample_bytes_count & 0xFF;
  wav_header[DATA_SIZE_OFFSET + 1] = (sample_bytes_count >> 8) & 0xFF;
  wav_header[DATA_SIZE_OFFSET + 2] = (sample_bytes_count >> 16) & 0xFF;
  wav_header[DATA_SIZE_OFFSET + 3] = (sample_bytes_count >> 24) & 0xFF;

  // Replace data size placeholder
  uint32_t total_file_size = sample_bytes_count + WAV_HEADER_SIZE_IN_BYTES - 8;
  wav_header[FILE_SIZE_OFFSET] = total_file_size & 0xFF;
  wav_header[FILE_SIZE_OFFSET + 1] = (total_file_size >> 8) & 0xFF;
  wav_header[FILE_SIZE_OFFSET + 2] = (total_file_size >> 16) & 0xFF;
  wav_header[FILE_SIZE_OFFSET + 3] = (total_file_size >> 24) & 0xFF;

  size_t written_header_bytes =
      fwrite(wav_header, 1, WAV_HEADER_SIZE_IN_BYTES, wav_file);
  if (written_header_bytes != WAV_HEADER_SIZE_IN_BYTES) {
    printf("Could not write every header byte (%lu out of %d)\n",
           written_header_bytes, WAV_HEADER_SIZE_IN_BYTES);
    fclose(wav_file);
    return WRITE_WAV_COULD_NOT_WRITE_HEADER;
  }

  size_t written_sample_bytes =
      fwrite(data->sample_bytes, 1, sample_bytes_count, wav_file);
  if (written_sample_bytes != sample_bytes_count) {
    printf("Could not write every data byte (%lu out of %d)\n",
           written_sample_bytes, sample_bytes_count);
    fclose(wav_file);
    return WRITE_WAV_COULD_NOT_WRITE_DATA;
  }

  fclose(wav_file);
  return WRITE_WAV_SUCCESS;
}
