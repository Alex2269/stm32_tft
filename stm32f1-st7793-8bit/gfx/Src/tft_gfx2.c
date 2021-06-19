#include "tft_gfx2.h"

void fastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  setAddrWindow(x, y, w, h);
  tft_write_bus(color);
  GPIOB_Lo = 1<<CS;
  for (uint32_t i = 0; i < w * h; i++)
  {
    tft_wr_pin_strob2();
  }
  GPIOB_Hi = 1<<CS;
}
