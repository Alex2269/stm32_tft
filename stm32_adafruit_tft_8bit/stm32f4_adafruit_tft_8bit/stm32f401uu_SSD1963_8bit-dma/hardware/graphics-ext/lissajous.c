#include "vga_adafruit.h"
#include "lissajous.h"

void lissajous(uint16_t pos_x, uint16_t pos_y, float frequency1, float frequency2, uint32_t color, uint32_t bg_color)
{
  #define scale 18
  float x = 0;
  float y = 0;
  float arx[628] = {0};
  float ary[628] = {0};
  int angle = 0;
  float beta = 0.0;
  // Frame Loop
  while(1)
  {
    // Curve Draw Loop
    for(angle = 0; angle < 628; angle ++)
    {
      setCursor(x, y);
      arx[angle] = 4*cos(frequency1 * (angle + beta) + 60);
      ary[angle] = 2*sin(frequency2 * angle + 30);
      fillRect(pos_x + arx[angle] * scale , pos_y + ary[angle] * scale ,1,1, color);
    }
    for(uint32_t i=0;i<5e5; i++) __asm volatile ("nop");// delay(10); // Refresh Rate

    for(angle = 0; angle < 628; angle ++)
    {
      fillRect(pos_x + arx[angle] * scale , pos_y + ary[angle] * scale ,1,1, bg_color); // clear
    }
    beta += 0.05;
  }
}
