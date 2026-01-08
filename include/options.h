#include <stdint.h>

#ifndef ZT_OPTIONS_H
#define ZT_OPTIONS_H

typedef struct Options {
  uint16_t channel_count;
  uint16_t bits_per_sample;
  uint32_t sample_rate;
} Options;

Options parse_options(int argc, char **argv);

#endif
