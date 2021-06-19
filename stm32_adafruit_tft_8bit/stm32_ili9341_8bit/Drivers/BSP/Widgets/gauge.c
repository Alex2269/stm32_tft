#include "gauge.h"

#define M_PI 3.14159

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

uint8_t char2digit(const char* p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void GaugeWritePixel(uint16_t x,uint16_t y,uint16_t color)
{
  tft_address_set(x,y,x,y);
  LCD_IO_WriteData(color>>8);
  LCD_IO_WriteData(color&0x00ff);
}

uint16_t new_value;

void gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t rotate, uint16_t color, uint16_t val)
{
  float sx = 0, sy = 1;    // Saved x & y multipliers
  float beg_sx, beg_sy, end_sx, end_sy;   // Saved x & y coords

  coordinate_x += 120;
  coordinate_y += 120;

  float sdeg=0 ;
  uint16_t x00=0, x11=0, y00=0, y11=0, x22=0, y22=0;

  // Draw gauge
  for(uint16_t i = 270; i<=370; i += 10)
  {
    sx = cos((i - rotate)*0.0174532925);
    sy = sin((i - rotate)*0.0174532925);

    x00 = sx*102+coordinate_x; // coordinates for points
    y00 = sy*102+coordinate_y;

    x11 = sx*118+coordinate_x; // coordinates for lines
    y11 = sy*118+coordinate_y;

    x22 = sx*132+coordinate_x; // coordinates for letters
    y22 = sy*132+coordinate_y;

    // Draw minute markers
    //GaugeWritePixel(x00, y00, LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    if(i==270||i==360) BSP_LCD_FillCircle(x00, y00, 3); // draw min & max red points
    // Draw main quadrant dots
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    if(i%90) BSP_LCD_FillCircle(x00, y00, 2);

    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    BSP_LCD_DrawLine(x00, y00, x11, y11); // draw small lines

    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayChar(x22, y22, ('0'-27+i/10)); // draw letters
  }

  BSP_LCD_FillCircle(coordinate_x, coordinate_y, 5); // draw center circle

//   BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//   BSP_LCD_DrawLine(end_sx, end_sy, beg_sx, beg_sy); // Erase old ray

  // Pre-compute hand degrees, x & y coords for a fast screen update
  sdeg = val*1; // 0-59 -> 0-354
  sx = cos((sdeg - rotate)*0.0174532925);
  sy = sin((sdeg - rotate)*0.0174532925);

  beg_sx = sx*10+coordinate_x; // center x point
  beg_sy = sy*10+coordinate_y; // center y point

  end_sx = sx*95+coordinate_x; // end x point
  end_sy = sy*95+coordinate_y; // end y point

  BSP_LCD_SetTextColor(color);
  BSP_LCD_DrawLine(beg_sx,beg_sy, end_sx,end_sy); // draw ray

  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillCircle(coordinate_x, coordinate_y, 5);
}
