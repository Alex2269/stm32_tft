#include "tft_adafruit.h"
#include "tft_gfx.h"
#include "tft_gfx2.h"

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
  {
    swap(x0, y0);
    swap(x1, y1);
  }
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
    if (steep)
    {
      writePixel(y0, x0, color);
    }
    else
    {
      writePixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void startWrite(void)
{
  GPIOB_Lo = 1<<CS;
}

void endWrite(void)
{
  GPIOB_Hi = 1<<CS;
}

void writePixel(int16_t x, int16_t y, uint16_t color)
{
  draw_pixel(x, y, color);
}

void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  drawFastVLine(x, y, h, color);
}

void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  drawFastHLine(x, y, w, color);
}

void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  fillRect(x,y,w,h,color);
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  startWrite();
  writeLine(x, y, x, y+h-1, color);
  endWrite();
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  startWrite();
  writeLine(x, y, x+w-1, y, color);
  endWrite();
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  setAddrWindow(x, y, w, h);
  tft_write_bus(color);
  GPIOB_Lo = 1<<CS;
  for (int16_t i=x; i<x+w; i++)
  {
    writeFastVLine(i, y, h, color);
  }
  GPIOB_Hi = 1<<CS;
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  // Update in subclasses if desired!
  if(x0 == x1)
  {
    if(y0 > y1) swap(y0, y1);
    drawFastVLine(x0, y0, y1 - y0 + 1, color);
  }
  else if(y0 == y1)
  {
    if(x0 > x1) swap(x0, x1);
    drawFastHLine(x0, y0, x1 - x0 + 1, color);
  }
  else 
  {
    startWrite();
    writeLine(x0, y0, x1, y1, color);
    endWrite();
  }
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  startWrite();
  writePixel(x0 , y0+r, color);
  writePixel(x0 , y0-r, color);
  writePixel(x0+r, y0 , color);
  writePixel(x0-r, y0 , color);
  while (x<y)
  {
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
  endWrite();
}

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4)
    {
      writePixel(x0 + x, y0 + y, color);
      writePixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2)
    {
      writePixel(x0 + x, y0 - y, color);
      writePixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8)
    {
      writePixel(x0 - y, y0 + x, color);
      writePixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1)
    {
      writePixel(x0 - y, y0 - x, color);
      writePixel(x0 - x, y0 - y, color);
    }
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  startWrite();
  writeFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
  endWrite();
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x1)
    {
      writeFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      writeFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2)
    {
      writeFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      writeFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  startWrite();
  writeFastHLine(x, y, w, color);
  writeFastHLine(x, y+h-1, w, color);
  writeFastVLine(x, y, h, color);
  writeFastVLine(x+w-1, y, h, color);
  endWrite();
}

void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  startWrite();
  writeFastHLine(x+r , y , w-2*r, color);
  writeFastHLine(x+r , y+h-1, w-2*r, color);
  writeFastVLine(x , y+r , h-2*r, color);
  writeFastVLine(x+w-1, y+r , h-2*r, color);
  drawCircleHelper(x+r , y+r , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r , y+h-r-1, r, 8, color);
  endWrite();
}

void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  startWrite();
  writeFillRect(x+r, y, w-2*r, h, color);
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r , y+r, r, 2, h-2*r-1, color);
  endWrite();
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) 
  {
    swap(y0, y1); swap(x0, x1);
  }
  //--
  if (y1 > y2) 
  {
    swap(y2, y1); swap(x2, x1);
  }
  //--
  if (y0 > y1) 
  {
    swap(y0, y1); swap(x0, x1);
  }
  //--
  startWrite();
  if(y0 == y2) 
  {
    // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    writeFastHLine(a, y0, b-a+1, color);
    endWrite();
    return;
  }
  //--
  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t
  sa = 0,
  sb = 0;
  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2. If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1; // Include y1 scanline
  else last = y1-1; // Skip it
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
    if(a > b) swap(a,b);
    writeFastHLine(a, y, b-a+1, color);
  }
  //--
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
    if(a > b) swap(a,b);
    writeFastHLine(a, y, b-a+1, color);
  }
  //--
  endWrite();
}

void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++)
    {
      if(i & 7) byte <<= 1;
      else byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      if(byte & 0x80) writePixel(x+i, y, color);
    }
  }
  endWrite();
}

void drawBitmap2(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      writePixel(x+i, y, (byte & 0x80) ? color : bg);
    }
  }
  endWrite();
}

void drawBitmap3(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = bitmap[j * byteWidth + i / 8];
      if(byte & 0x80) writePixel(x+i, y, color);
    }
  }
  endWrite();
}

void drawBitmap4(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = bitmap[j * byteWidth + i / 8];
      writePixel(x+i, y, (byte & 0x80) ? color : bg);
    }
  }
  endWrite();
}

void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte >>= 1;
      else byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      if(byte & 0x01) writePixel(x+i, y, color);
    }
  }
  endWrite();
}

void drawGrayscaleBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
    }
  }
  endWrite();
}

void drawGrayscaleBitmap2(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      writePixel(x+i, y, bitmap[j * w + i]);
    }
  }
  endWrite();
}

void drawGrayscaleBitmap3(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = pgm_read_byte(&mask[j * bw + i / 8]);
      if(byte & 0x80)
      {
        writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
      }
    }
  }
  endWrite();
}

void drawGrayscaleBitmap4(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = mask[j * bw + i / 8];
      if(byte & 0x80)
      {
        writePixel(x+i, y, bitmap[j * w + i]);
      }
    }
  }
  endWrite();
}

void drawRGBBitmap1(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      writePixel(x+i, y, pgm_read_word(&bitmap[j * w + i]));
    }
  }
  endWrite();
}

void drawRGBBitmap2(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      writePixel(x+i, y, bitmap[j * w + i]);
    }
  }
  endWrite();
}

void drawRGBBitmap3(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = pgm_read_byte(&mask[j * bw + i / 8]);
      if(byte & 0x80)
      {
        writePixel(x+i, y, pgm_read_word(&bitmap[j * w + i]));
      }
    }
  }
  endWrite();
}

void drawRGBBitmap4(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8;
  uint8_t byte = 0;
  startWrite();
  for(int16_t j=0; j<h; j++, y++)
  {
    for(int16_t i=0; i<w; i++ )
    {
      if(i & 7) byte <<= 1;
      else byte = mask[j * bw + i / 8];
      if(byte & 0x80)
      {
        writePixel(x+i, y, bitmap[j * w + i]);
      }
    }
  }
  endWrite();
}

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
  if(!gfxFont)
  {
    if((x >= tft_W) ||
    (y >= tft_H) ||
    ((x + 6 * size - 1) < 0) ||
    ((y + 8 * size - 1) < 0))
    return;
    // if(!_cp437 && (c >= 176)) c++;
    startWrite();
    for(int8_t i=0; i<5; i++ )
    {
      uint8_t line = pgm_read_byte(&font[c * 5 + i]);
      for(int8_t j=0; j<8; j++, line >>= 1)
      {
        if(line & 1)
        {
          if(size == 1)
          writePixel(x+i, y+j, color);
          else
          writeFillRect(x+i*size, y+j*size, size, size, color);
        }
        else if(bg != color)
        {
          if(size == 1)
          writePixel(x+i, y+j, bg);
          else
          writeFillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
    }
    if(bg != color)
    {
      if(size == 1) writeFastVLine(x+5, y, 8, bg);
      else writeFillRect(x+5*size, y, size, 8*size, bg);
    }
    endWrite();
  }
  else
  {
    c -= (uint8_t)pgm_read_byte(&gfxFont->first);
    GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
    uint8_t *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t w = pgm_read_byte(&glyph->width);
    uint8_t h = pgm_read_byte(&glyph->height);
    int8_t xo = pgm_read_byte(&glyph->xOffset);
    int8_t yo = pgm_read_byte(&glyph->yOffset);
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;
    if(size > 1)
    {
      xo16 = xo;
      yo16 = yo;
    }
    startWrite();
    for(yy=0; yy<h; yy++)
    {
      for(xx=0; xx<w; xx++)
      {
        if(!(bit++ & 7))
        {
          bits = pgm_read_byte(&bitmap[bo++]);
        }
        if(bits & 0x80)
        {
          if(size == 1)
          {
            writePixel(x+xo+xx, y+yo+yy, color);
          }
          else
          {
            writeFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
          }
        }
        bits <<= 1;
      }
    }
    endWrite();
  }
}

void write(uint8_t c)
{
  if(!gfxFont)
  {
    // 'Classic' built-in font
    if(c == '\n')
    {
      // Newline?
      cursor_x = 0; // Reset x to zero,
      cursor_y += textsize * 8; // advance y one line
    }
    else if(c != '\r')
    {
      // Ignore carriage returns
      if(wrap && ((cursor_x + textsize * 6) > _width))
      {
        // Off right?
        cursor_x = 0; // Reset x to zero,
        cursor_y += textsize * 8; // advance y one line
      }
      drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
      cursor_x += textsize * 6; // Advance x one char
    }
  }
  else
  {
    // Custom font
    if(c == '\n')
    {
      cursor_x = 0;
      cursor_y += (int16_t)textsize *
      (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    }
    else if(c != '\r')
    {
      uint8_t first = pgm_read_byte(&gfxFont->first);
      if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last)))
      {
        GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c - first]);
        uint8_t w = pgm_read_byte(&glyph->width);
        uint8_t h = pgm_read_byte(&glyph->height);
        if((w > 0) && (h > 0))
        {
          // Is there an associated bitmap?
          int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
          if(wrap && ((cursor_x + textsize * (xo + w)) > _width))
          {
            cursor_x = 0;
            cursor_y += (int16_t)textsize *
            (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
          }
          drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
        }
        cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
      }
    }
  }
}

void setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

int16_t getCursorX(void)
{
  return cursor_x;
}

int16_t getCursorY(void)
{
  return cursor_y;
}

void setTextSize(uint8_t s)
{
  textsize = (s > 0) ? s : 1;
}

void setTextColor(uint16_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void setTextColor_bg(uint16_t c, uint16_t bg)
{
  textcolor   = c;
  textbgcolor = bg;
}

void setFont(const GFXfont *f)
{
  if(f)
  {
    // Font struct pointer passed in?
    if(!gfxFont)
    {
      // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  }
  else if(gfxFont)
  {
    // NULL passed. Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *)f;
}

// Broke this out as it's used by both the PROGMEM- and RAM-resident
// getTextBounds() functions.
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy)
{
  if(gfxFont)
  {
    if(c == '\n')
    {
      // Newline?
      *x = 0; // Reset x to zero, advance y by one line
      *y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    }
    else if(c != '\r')
    {
      // Not a carriage return; is normal char
      uint8_t first = pgm_read_byte(&gfxFont->first),
      last = pgm_read_byte(&gfxFont->last);
      if((c >= first) && (c <= last))
      {
        // Char present in this font?
        GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c - first]);
        uint8_t gw = pgm_read_byte(&glyph->width);
        uint8_t gh = pgm_read_byte(&glyph->height);
        uint8_t xa = pgm_read_byte(&glyph->xAdvance);
        int8_t xo = pgm_read_byte(&glyph->xOffset);
        int8_t yo = pgm_read_byte(&glyph->yOffset);

        if(wrap && ((*x+(((int16_t)xo+gw)*textsize)) > _width))
        {
          *x = 0; // Reset x to zero, advance y by one line
          *y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }

        int16_t ts = (int16_t)textsize,
        x1 = *x + xo * ts,
        y1 = *y + yo * ts,
        x2 = x1 + gw * ts - 1,
        y2 = y1 + gh * ts - 1;

        if(x1 < *minx) *minx = x1;
        if(y1 < *miny) *miny = y1;
        if(x2 > *maxx) *maxx = x2;
        if(y2 > *maxy) *maxy = y2;
        *x += xa * ts;
      }
    }
  }
  else
  {
    // Default font
    if(c == '\n')
    {
      // Newline?
      *x = 0; // Reset x to zero,
      *y += textsize * 8; // advance y one line
      // min/max x/y unchaged -- that waits for next 'normal' character
    }
    else if(c != '\r')
    {
      // Normal char; ignore carriage returns
      if(wrap && ((*x + textsize * 6) > _width))
      {
        // Off right?
        *x = 0; // Reset x to zero,
        *y += textsize * 8; // advance y one line
      }

      int x2 = *x + textsize * 6 - 1, // Lower-right pixel of char
      y2 = *y + textsize * 8 - 1;

      if(x2 > *maxx) *maxx = x2; // Track max x, y
      if(y2 > *maxy) *maxy = y2;
      if(*x < *minx) *minx = *x; // Track min x, y
      if(*y < *miny) *miny = *y;
      *x += textsize * 6; // Advance x one char
    }
  }
}

// Pass string and a cursor position, returns UL corner and W,H.
void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
  uint8_t c; // Current character
  *x1 = x;
  *y1 = y;
  *w = *h = 0;
  int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;
  while((c = *str++))
  charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
  if(maxx >= minx)
  {
    *x1 = minx;
    *w = maxx - minx + 1;
  }

  if(maxy >= miny)
  {
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

// Same as above, but for PROGMEM strings
void getTextBounds2(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
  uint8_t *s = (uint8_t *)str, c;
  *x1 = x;
  *y1 = y;
  *w = *h = 0;
  int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;
  while((c = pgm_read_byte(s++)))
  charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
  if(maxx >= minx)
  {
    *x1 = minx;
    *w = maxx - minx + 1;
  }

  if(maxy >= miny)
  {
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

// Return the size of the display (per current rotation)
int16_t width(void)
{
  return _width;
}

int16_t height(void)
{
  return _height;
}
