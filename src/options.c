#include "../include/options.h"
#include "../include/wav.h"
#include <bits/getopt_core.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Options *parse_options(int argc, char **argv) {
  Options *opts = malloc(sizeof(Options));
  if (opts == NULL) {
    printf("Could not allocate memmory for options\n");
    return NULL;
  }

  opts->channel_count = STEREO_CHANNEL;
  opts->bits_per_sample = DEFAULT_SAMPLE_SIZE_IN_BITS;
  opts->sample_rate = CD_SAMPLE_RATE;

  uint32_t sample_rate = 0;
  uint16_t channel_count = 0;
  uint16_t bits_per_sample = 0;

  int opt;
  while ((opt = getopt(argc, argv, "c:b:s:")) != -1) {
    switch (opt) {
    case 'c':
      channel_count = atoi(optarg);
      break;
    case 'b':
      bits_per_sample = atoi(optarg);
      break;
    case 's':
      sample_rate = atoi(optarg);
      break;
    default:
      // Ignore unknown arguments
      break;
    }
  }

  if (channel_count == STEREO_CHANNEL || channel_count == MONO_CHANNEL) {
    opts->channel_count = channel_count;
  }

  if (bits_per_sample == LOWEST_SAMPLE_SIZE_IN_BITS ||
      bits_per_sample == DEFAULT_SAMPLE_SIZE_IN_BITS ||
      bits_per_sample == HIGHEST_SAMPLE_SIZE_IN_BITS) {
    opts->bits_per_sample = bits_per_sample;
  }

  if (sample_rate == CD_SAMPLE_RATE || sample_rate == DVD_SAMPLE_RATE ||
      sample_rate == HIGH_END_SAMPLE_RATE) {
    opts->sample_rate = sample_rate;
  }

  return opts;
}
