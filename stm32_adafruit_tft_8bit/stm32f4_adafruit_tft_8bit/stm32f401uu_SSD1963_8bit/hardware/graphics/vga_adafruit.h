#ifndef vga_adafruit_H
#define vga_adafruit_H

/* Include Library */
#include "stdint.h"
#include "stdbool.h"

#include "ftoa_impl.h"

/* -------------------------------------------------------------------------------- */
/* -- DEFINES                                                                    -- */
/* -------------------------------------------------------------------------------- */
#ifndef NULL
   #define NULL ((void*) 0)
#endif

#ifndef swap
  #define swap(a, b)    { uint16_t  t=a; a=b; b=t; }
#endif


void writePixel(uint16_t xp, uint16_t yp, uint32_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint32_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color);
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);


#endif // vga_adafruit_H


