#ifndef __xpt2046_H
#define __xpt2046_H

#include "soft_spi_ts.h"

#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct TS_Point
{
  uint8_t str_x[8];
  uint8_t str_y[8];
  uint8_t str_z[8];

  float x, y, z;
} TS_Point;

void pin_pen_on(void);
void get_point(TS_Point *p);

#endif /* __xpt2046_H */
