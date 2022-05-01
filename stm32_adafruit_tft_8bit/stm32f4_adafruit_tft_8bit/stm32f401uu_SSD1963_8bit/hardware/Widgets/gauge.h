#ifndef __gauge_H
#define __gauge_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

#include "vga_adafruit.h"

struct
{
  float pointX[400];
  float pointY[400];
} ga;

void draw_digital(int64_t value);
void gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t rotate, uint32_t color, uint16_t val);

#ifdef __cplusplus
}
#endif

#endif // __gauge_H
