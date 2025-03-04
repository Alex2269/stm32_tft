#include "vga_adafruit.h"
#include "SSD1963.h"
#include "io_SSD1963.h"

void writePixel(uint16_t xp, uint16_t yp, uint32_t color)
{
  DMA_Pixel_Set(xp, yp, color);
}

static inline void
writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  // --
  if (steep)
  {
    swap(x0, y0);
    swap(x1, y1);
  }
  // --
  if (x0 > x1)
  {
    swap(x0, x1);
    swap(y0, y1);
  }
  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int16_t err = dx / 2;
  int16_t ystep;
  // --
  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }
  for (; x0<=x1; x0++)
  {
    // --
    if (steep)
    {
      writePixel(y0, x0, color);
    }
    else
    {
      writePixel(x0, y0, color);
    }
    err -= dy;
    // --
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
  // Update in subclasses if desired!
  // --
  if(x0 == x1)
  {
    // --
    if(y0 > y1) swap(y0, y1);
    drawFastVLine(x0, y0, y1 - y0 + 1, color);
  }
  else if(y0 == y1)
  {
    // --
    if(x0 > x1) swap(x0, x1);
    drawFastHLine(x0, y0, x1 - x0 + 1, color);
  }
  else
  {
    writeLine(x0, y0, x1, y1, color);
  }
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color)
{
  DMA_WindowSet(x,x,y,y+h);
  for (uint16_t loopy = 0 ; loopy<=h; loopy++)
  {
    DMA_WriteData24(color);
  }
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color)
{
  DMA_WindowSet(x,x+w,y,y);
  for (uint16_t loopx = 0 ; loopx<=w; loopx++)
  {
    DMA_WriteData24(color);
  }
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
  uint16_t loopx, loopy;
  DMA_WindowSet(x,x+w,y,y+h);
  for (loopx = x; loopx < x+w + 1; loopx++)
  {
    for (loopy = y; loopy < y+h + 1; loopy++)
    {
      DMA_WriteData24(color);
    }
  }
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  writePixel(x0 , y0+r, color);
  writePixel(x0 , y0-r, color);
  writePixel(x0+r, y0 , color);
  writePixel(x0-r, y0 , color);
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    writePixel(x0 + x, y0 + y, color);
    writePixel(x0 - x, y0 + y, color);
    writePixel(x0 + x, y0 - y, color);
    writePixel(x0 - x, y0 - y, color);
    writePixel(x0 + y, y0 + x, color);
    writePixel(x0 - y, y0 + x, color);
    writePixel(x0 + y, y0 - x, color);
    writePixel(x0 - y, y0 - x, color);
  }
}

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint32_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // --
    if (cornername & 0x4)
    {
      writePixel(x0 + x, y0 + y, color);
      writePixel(x0 + y, y0 + x, color);
    }
    // --
    if (cornername & 0x2)
    {
      writePixel(x0 + x, y0 - y, color);
      writePixel(x0 + y, y0 - x, color);
    }
    // --
    if (cornername & 0x8)
    {
      writePixel(x0 - y, y0 + x, color);
      writePixel(x0 - x, y0 + y, color);
    }
    // --
    if (cornername & 0x1)
    {
      writePixel(x0 - y, y0 - x, color);
      writePixel(x0 - x, y0 - y, color);
    }
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color)
{
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    // --
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // --
    if (cornername & 0x1)
    {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    // --
    if (cornername & 0x2)
    {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color)
{
  drawFastHLine(x+r , y , w-2*r, color);
  drawFastHLine(x+r , y+h-1, w-2*r, color);
  drawFastVLine(x , y+r , h-2*r, color);
  drawFastVLine(x+w-1, y+r , h-2*r, color);
  drawCircleHelper(x+r , y+r , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r , y+h-r-1, r, 8, color);
}

void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint32_t color)
{
  fillRect(x+r, y, w-2*r, h, color);
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r , y+r, r, 2, h-2*r-1, color);
}

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  // --
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  // --
  if (y1 > y2)
  {
    swap(y2, y1); swap(x2, x1);
  }
  // --
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  // --
  if(y0 == y2)
  {
    // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    // --
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    // --
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }
  // --
  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t
  sa = 0, sb = 0;
  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2. If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  // --
  if(y1 == y2) last = y1; // Include y1 scanline
  else last = y1-1; // Skip it
  // --
  for(y=y0; y<=last; y++)
  {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    // --
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
  // --
  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2. This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++)
  {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    // --
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
  // --
}

