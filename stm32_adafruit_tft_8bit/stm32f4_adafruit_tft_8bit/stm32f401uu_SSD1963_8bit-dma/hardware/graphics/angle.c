#include "stdlib.h"
#include "vga_adafruit.h"
#include "SSD1963.h"
#include "angle.h"

/**
 * function to convert polar point to x y coordinates:
 * function gets radius and angle spaced from x1 y1
 * returns x2 y2 coordinates
**/
void get_angle_pos(Point * p)
{
  // static const float m_pi = 3.14159265359;
  // static const float half_circle = 180.0;
  // static const float degree = m_pi / half_circle;
  static const float degree = 0.0174532925199;
  p->x2 = p->ro * sin((p->angle + 90)*degree) + p->x1;
  p->y2 = p->ro * cos((p->angle + 90)*degree) + p->y1;
}

void draw_angle(Point * p, int16_t ro, uint8_t solid, uint32_t color)
{
  static float arx[361]; // array of sinus angles
  static float ary[361]; // array of cosinus angles
  static const float degree = 0.0174532925199; // PI/180

  if(p->angle<0) p->angle = abs(p->angle);
  if(p->angle>360) p->angle = p->angle % 360;

  float fx = ro * arx[p->angle] + p->x1;
  float fy = ro * ary[p->angle] + p->y1;

  fillRect(fx, fy, solid, solid, color);

  /*
   * create angle array
   * Exceptions to regular calculations
   * we calculate only once
   */
  static volatile uint8_t flag_calc = 0;
  if(flag_calc) return;
  flag_calc=1;
  for(uint16_t i = 0; i<=360; i++)
  {
    arx[i] = sin((i + 90)*degree); // create an array of sinus angles
    ary[i] = cos((i + 90)*degree); // create an array of cosinus angles
  }
}

void draw_arrow(Point * p, int16_t ro, uint8_t solid, uint32_t color)
{
  static float arx[721]; // array of sinus angles
  static float ary[721]; // array of cosinus angles
  static const float degree = 0.0174532925199; // PI/180

  if(p->angle<0) p->angle = abs(p->angle);
  if(p->angle>360) p->angle = p->angle % 360;

  float Ax = 8 * arx[360+p->angle+55] + p->x1;
  float Ay = 8 * ary[360+p->angle+55] + p->y1;

  float Bx = ro * arx[p->angle] + p->x1;
  float By = ro * ary[p->angle] + p->y1;

  float Cx = 8 * arx[360+p->angle-55] + p->x1;
  float Cy = 8 * ary[360+p->angle-55] + p->y1;

  /* fillRect(Ax, Ay, solid, solid, color);
  fillRect(Bx, By, solid, solid, color);
  fillRect(Cx, Cy, solid, solid, color); */
  fillTriangle(Ax, Ay, Bx, By, Cx, Cy, color); // same draw arrow

  fillCircle(p->x1, p->y1, 10, CYAN); // draw big circle on bottom arrow
  float Smx = (ro-8) * arx[p->angle] + p->x1; // small circle
  float Smy = (ro-8) * ary[p->angle] + p->y1; // small circle
  fillCircle(Smx, Smy, 2, color); // draw small circle on top arrow

  /*
   * create angle array
   * Exceptions to regular calculations
   * we calculate only once
   */
  static volatile uint8_t flag_calc = 0;
  if(flag_calc) return;
  flag_calc=1;
  for(int16_t i = -360; i<=360; i++)
  {
    arx[i+360] = sin((i + 90)*degree); // create an array of sinus angles
    ary[i+360] = cos((i + 90)*degree); // create an array of cosinus angles
  }
}

