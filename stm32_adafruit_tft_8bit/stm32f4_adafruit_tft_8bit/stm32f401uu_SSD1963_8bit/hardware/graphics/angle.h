#ifndef __angle_h
#define __angle_h

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

#ifndef swap
  #define swap(a, b)    { uint16_t  t=a; a=b; b=t; }
#endif

typedef struct
{
  uint16_t x1;
  uint16_t y1;
  uint16_t x2;
  uint16_t y2;
  int16_t angle;
  int16_t ro;
} Point;

void get_angle_pos(Point * p);
void draw_angle(Point * p, int16_t ro, uint8_t solid, uint32_t color);
void draw_arrow(Point * p, int16_t ro, uint8_t solid, uint32_t color);

#endif // __angle_h

