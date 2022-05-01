#include "bitmap.h"
#include "SSD1963.h"
#include "io_SSD1963.h"

void drawRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap)
{
  uint32_t offset;
  uint32_t SizeBMP,Width,Height,ColBPP;

  uint8_t *src = (uint8_t*)bitmap;

  //   SizeBMP = src[0x02] | src[0x02+1]<<8 | src[0x02+2]<<16 | src[0x02+3]<<24;
  //   offset  = src[0x0a] | src[0x0a+1]<<8 | src[0x0a+2]<<16 | src[0x0a+3]<<24;
  //   Width   = src[0x12] | src[0x12+1]<<8 | src[0x12+2]<<16 | src[0x12+3]<<24;
  //   Height  = src[0x16] | src[0x16+1]<<8 | src[0x16+2]<<16 | src[0x16+3]<<24;
  //   ColBPP  = src[0x1C] | src[0x1C+1]<<8;

  Width   =  src[0] | src[1]<<8 | src[2]<<16 | src[3]<<24;
  Height  =  src[4] | src[5]<<8 | src[6]<<16 | src[7]<<24;

  SizeBMP = Width*Height;
  ColBPP  = src[8] | src[9]<<8;
  offset  =  src[15];

  // debug:
  /** setTextColor(YELLOW);
  setTextBgColor(0);
  setTextSize(1);
  UG_FONT_set(&font_ClearSans_28X30);
  setCursor(64, 340);

  setCursor(16, 240); val2hex(Width);
  setCursor(16, 270); val2hex(Height);
  setCursor(16, 300); draw_value(SizeBMP);
  setCursor(16, 330); draw_value(offset);
  setCursor(16, 360); val2hex(ColBPP); **/

  uint8_t alpha,red,green,blue;
  uint32_t color;
  uint32_t pix=0;

  *src = (uint8_t*)bitmap;
  // pix=SizeBMP;
  pix=0;
  Display_WindowSet(x, x+Width-1, y, y+Height-1);
  for(int16_t j=0;j<Height; j++)
  {
    for(int16_t i=0; i<Width; i++)
    {
      red    =  src[offset+pix++];
      green  =  src[offset+pix++];
      blue   =  src[offset+pix++];

      tft_write_data(red);
      tft_write_data(green);
      tft_write_data(blue);
      // color = convertRGBto888(red, green, blue);
      // writePixel(x+i, y+j, color);
    }
  }
}
