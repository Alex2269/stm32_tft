#include "stdlib.h"
#include "vga_adafruit.h"
#include "SSD1963.h"
#include "arch.h"

void draw_arch(uint16_t x, uint16_t y, int16_t fi_beg, int16_t fi_end, int16_t ro, uint8_t solid, uint32_t color)
{
  // static const float m_pi = 3.14159265359;
  // static const float half_circle = 180.0;
  // static const float degree = m_pi / half_circle;
  static const float degree = 0.0174532925199;

  if(fi_beg>fi_end) swap(fi_beg,fi_end);

  for (int a = fi_beg; a <= fi_end; a++)  // draw arc`s:
  {
    float fx = ro * sin((a + 90)*degree);
    float fy = ro * cos((a + 90)*degree);
    fillRect(x + fx, y + fy, solid, solid, color);
  }
}
