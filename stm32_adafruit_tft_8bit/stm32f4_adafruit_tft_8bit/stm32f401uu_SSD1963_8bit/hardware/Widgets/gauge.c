
#include "gauge.h"
#include "fonts_all_includes.h"
#include "SSD1963.h"

// #pragma GCC diagnostic ignored "-Wunused-variable"
// #pragma GCC diagnostic ignored "-Wunused-function"

void draw_digital(int64_t value)
{
  uint8_t strval[16];
  utoa_local((int64_t)value, strval, 10);
  UG_PutString(strval);
}

void init_gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t scale)
{
  static volatile uint8_t graflag = 0 ;
  float sx = 0, sy = 0; // Saved x & y multipliers

  uint16_t x00=0, x11=0, y00=0, y11=0, x22=0, y22=0;

  coordinate_x += scale;
  coordinate_y += scale;

  //  if(graflag) return;
  //  graflag=1;

  for(uint16_t i = 0; i<=380; i ++)
  {
    ga.pointX[i] =   cos((i - 100)*0.0174532925);
    ga.pointY[i] =   sin((i - 100)*0.0174532925);
  }

  for(uint8_t distance = 10; distance <60; distance+=5)
  {
    for (int a = 270; a <= 370; a+=5)  // draw arc`s:
    {
      float x =  distance * cos((a - 100)*0.0174532925);
      float y =  distance * sin((a - 100)*0.0174532925);
      // writePixel(coordinate_x + x + x, coordinate_y +  y + y, CYAN);
      // fillRect(coordinate_x + x + x, coordinate_y +  y + y, (uint8_t)distance*0.05,(uint8_t)distance*0.05, CYAN);
      drawCircle(coordinate_x + x + x, coordinate_y +  y + y, (uint8_t)distance*0.05, CYAN);
    }
  }

  if(graflag) return;
  graflag=1;

  // Draw gauge
  for(uint16_t i = 270; i<=370; i += 10)
  {
    sx = ga.pointX[i];
    sy = ga.pointY[i];

    x00 = sx * (scale+4) + coordinate_x; // coordinates for lines
    y00 = sy * (scale+4) + coordinate_y;

    x11 = sx * (scale+8) + coordinate_x; // coordinates for bals
    y11 = sy * (scale+8) + coordinate_y;

    x22 = sx * (scale+36) + coordinate_x; // coordinates for letters
    y22 = sy * (scale+36) + coordinate_y;

    if(i==270||i==360) fillCircle(x00, y00, 1, RED); // draw min & max red points
    else fillCircle(x00, y00, 1, YELLOW); // draw min & max red points

    // if(i%90)fillCircle(x11, y11, 2, GREEN); // Draw center quadrant dots
    fillCircle(x11, y11, 2, GREEN); // Draw center quadrant dots

    drawLine(x00, y00, x11, y11 ,CYAN); // draw small lines

    setTextSize(1);
    // fillCircle(x22, y22, 12, GREEN); // Draw center quadrant dots
    setTextColor(YELLOW);
    setTextBgColor(0);
    UG_FONT_set(&font_ClearSans_22X22);
    setCursor(x22, y22);

    // UG_PutChar('0'+i/10-27); // draw letters
    draw_digital(i/10-27); // draw letters
  }
}

void gauge(uint16_t coordinate_x, uint16_t coordinate_y, uint16_t rotate, uint32_t color, uint16_t val)
{
  volatile static uint16_t scale = 120;
  init_gauge(coordinate_x, coordinate_y, scale);

  float sx = 0, sy = 0;    // Saved x & y multipliers
  float beg_sx, beg_sy, end_sx, end_sy;   // Saved x & y coords
  float bal_sx, bal_sy;   // Saved x & y coords fot bal

  static uint16_t  oldX1,oldY1,oldX2,oldY2,oldbalx,oldbaly; // for clean screen, saved old values

  coordinate_x += scale ;
  coordinate_y += scale ;

  // drawLine(oldX1,   oldY1,   oldX2, oldY2, BLACK); // Erase old arrow

  fillTriangle(oldX1+4, oldY1-4, oldX1-4, oldY1+4, oldX2, oldY2, BLACK); // Erase old arrow
  fillCircle(oldbalx, oldbaly, 2, BLACK);

  // Pre-compute hand degrees, x & y coords for a fast screen update

  sx = ga.pointX[val];
  sy = ga.pointY[val];

  beg_sx = sx * 2.0 + coordinate_x; // center x point
  beg_sy = sy * 2.0 + coordinate_y; // center y point

  end_sx = sx * (scale-2) + coordinate_x; // end x point
  end_sy = sy * (scale-2) + coordinate_y; // end y point

  bal_sx = sx * (scale-6) + coordinate_x; // bal x point
  bal_sy = sy * (scale-6) + coordinate_y; // bal y point

  // drawLine(beg_sx  ,beg_sy,   end_sx,end_sy, color); // draw arrow

  fillTriangle(beg_sx+4, beg_sy-4, beg_sx-4, beg_sy+4, end_sx, end_sy, color); // draw arrow
  fillCircle(bal_sx,bal_sy, 2, color);

  // save values for clean ray
  oldX1 = beg_sx; oldY1 = beg_sy;
  oldX2 = end_sx; oldY2 = end_sy;
  oldbalx = bal_sx;
  oldbaly = bal_sy;

  fillCircle(coordinate_x, coordinate_y, 4, YELLOW);
  drawCircle(coordinate_x, coordinate_y, 5, CYAN); // draw center circle
  drawCircle(coordinate_x, coordinate_y, 6, CYAN); // draw center circle
  init_gauge(coordinate_x-scale, coordinate_y-scale, scale);
}
