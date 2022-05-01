#ifndef __arch_h
#define __arch_h

#include "stdint.h"
#include "stdbool.h"

#ifndef swap
  #define swap(a, b)    { uint16_t  t=a; a=b; b=t; }
#endif

void draw_arch(uint16_t x, uint16_t y, int16_t fi_beg, int16_t fi_end, int16_t ro, uint8_t solid, uint32_t color);

#endif // __arch_h

