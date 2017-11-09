#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <inttypes.h>

typedef union {
  struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t clear;
  };
  uint32_t raw;
} RGBC_t;

// uint8_t color_values[];
void color_sensor_init(void);


#endif