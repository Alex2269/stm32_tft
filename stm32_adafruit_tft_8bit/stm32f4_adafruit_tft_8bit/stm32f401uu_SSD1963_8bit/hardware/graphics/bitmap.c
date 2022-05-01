#include "bitmap.h"
#include "SSD1963.h"
#include "io_SSD1963.h"

void drawRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap)
{
  uint32_t SizeBMP,offset,Width,Height;
  uint16_t ColBPP;
  uint8_t *src = (uint8_t*)bitmap;

  // for raw BMP:
  /* SizeBMP = src[0x02] | src[0x02+1]<<8 | src[0x02+2]<<16 | src[0x02+3]<<24;
     offset  = src[0x0a] | src[0x0a+1]<<8 | src[0x0a+2]<<16 | src[0x0a+3]<<24;
     Width   = src[0x12] | src[0x12+1]<<8 | src[0x12+2]<<16 | src[0x12+3]<<24;
     Height  = src[0x16] | src[0x16+1]<<8 | src[0x16+2]<<16 | src[0x16+3]<<24;
     ColBPP  = src[0x1c] | src[0x1c+1]<<8; */

  // for raw BMP:
  /* SizeBMP     = *(uint16_t *)(src + 0x02 );
     offset      = *(uint32_t *)(src + 0x0a );
     Width       = *(uint32_t *)(src + 0x12 );
     Height      = *(uint32_t *)(src + 0x16 );
     ColBPP      = *(uint16_t *)(src + 0x1c ); */


  // for prepared BMP:
  /* Width   =  src[0] | src[1]<<8 | src[2]<<16 | src[3]<<24;
     Height  =  src[4] | src[5]<<8 | src[6]<<16 | src[7]<<24;
     SizeBMP = Width*Height;
     ColBPP  = src[8] | src[9]<<8;
     offset  = src[15]; */

  // for prepared BMP:
  Width       = *(uint32_t *)(src + 0x00);
  Height      = *(uint32_t *)(src + 0x04);
  SizeBMP     =  Width * Height;
  ColBPP      = *(uint16_t *)(src + 0x08);
  offset      = *(uint8_t  *)(src + 0x0f);

  // debug:
  /* setTextColor(YELLOW);
     setTextBgColor(0);
     setTextSize(1);
     UG_FONT_set(&font_ClearSans_28X30);
     setCursor(64, 340);

     setCursor(16, 240); val2hex(Width);
     setCursor(16, 270); val2hex(Height);
     setCursor(16, 300); draw_value(SizeBMP);
     setCursor(16, 330); draw_value(offset);
     setCursor(16, 360); val2hex(ColBPP); */

  uint8_t alpha,red,green,blue;
  uint32_t color;
  uint32_t pix=0;

  // pix=SizeBMP;
  pix=0;
  Display_WindowSet(x, x+Width-1, y, y+Height-1);
  for(int16_t h=0; h<Height; h++)
  {
    for(int16_t w=0; w<Width; w++)
    {
      red    =  src[offset+pix++];
      green  =  src[offset+pix++];
      blue   =  src[offset+pix++];

      tft_write_data(red);
      tft_write_data(green);
      tft_write_data(blue);
      // color = convertRGBto888(red, green, blue);
      // writePixel(x+w, y+h, color);
    }
  }
}
