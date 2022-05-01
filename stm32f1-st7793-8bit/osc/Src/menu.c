#include "menu.h"
#include "amplifier.h"
#include "tft_gfx.h"
#include "tft_gfx2.h"

#define MARKRECT 2
#define FONT_H 16
#define FONT_W 8

#define ybar (tft_H-18)
#define offset 32
#define xbar 40

#define debug 0

/*
** draw string
*/
void write_str(uint16_t x, uint16_t y, int8_t* label)
{
  tft_puts8x16(y,x,label,TFT_STRING_MODE_BACKGROUND);
}

/*
** draw parameters
*/
void write_enc_val(uint16_t x, uint16_t y, int32_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_CYAN;
  #define FONT_H 16
  #define FONT_W 8
  char speed_value[16];
  utoa((int)value, speed_value, 10); // sprintf(speed_value,"%d",value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<100000000) tft_fill(y,x+FONT_W*8,y+FONT_H,x+FONT_W*9,BACK_COLOR); // x1 y1 x2 y2
  if(value<10000000) tft_fill(y,x+FONT_W*7,y+FONT_H,x+FONT_W*8,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000000) tft_fill(y,x+FONT_W*6,y+FONT_H,x+FONT_W*7,BACK_COLOR); // x1 y1 x2 y2
  if(value<100000) tft_fill(y,x+FONT_W*5,y+FONT_H,x+FONT_W*6,BACK_COLOR); // x1 y1 x2 y2
  if(value<10000) tft_fill(y,x+FONT_W*4,y+FONT_H,x+FONT_W*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000) tft_fill(y,x+FONT_W*3,y+FONT_H,x+FONT_W*4,BACK_COLOR); // x1 y1 x2 y2
  if(value<100) tft_fill(y,x+FONT_W*2,y+FONT_H,x+FONT_W*3,BACK_COLOR); // x1 y1 x2 y2
  if(value<10) tft_fill(y,x+FONT_W*1,y+FONT_H,x+FONT_W*2,BACK_COLOR); // x1 y1 x2 y2
  tft_puts8x16(y,x,(int8_t*)speed_value,TFT_STRING_MODE_BACKGROUND);
}

/*
** draw parameters
*/
void write_val(uint16_t x, uint16_t y, int32_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_YELLOW;
  char speed_value[16];
  utoa((int)value, speed_value, 10); // sprintf(speed_value,"%d",value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<100000000) tft_fill(y,x+FONT_W*8,y+FONT_H,x+FONT_W*9,BACK_COLOR); // x1 y1 x2 y2
  if(value<10000000) tft_fill(y,x+FONT_W*7,y+FONT_H,x+FONT_W*8,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000000) tft_fill(y,x+FONT_W*6,y+FONT_H,x+FONT_W*7,BACK_COLOR); // x1 y1 x2 y2
  if(value<100000) tft_fill(y,x+FONT_W*5,y+FONT_H,x+FONT_W*6,BACK_COLOR); // x1 y1 x2 y2
  if(value<10000) tft_fill(y,x+FONT_W*4,y+FONT_H,x+FONT_W*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000) tft_fill(y,x+FONT_W*3,y+FONT_H,x+FONT_W*4,BACK_COLOR); // x1 y1 x2 y2
  if(value<100) tft_fill(y,x+FONT_W*2,y+FONT_H,x+FONT_W*3,BACK_COLOR); // x1 y1 x2 y2
  if(value<10) tft_fill(y,x+FONT_W*1,y+FONT_H,x+FONT_W*2,BACK_COLOR); // x1 y1 x2 y2
  tft_puts8x16(y,x,(int8_t*)speed_value,TFT_STRING_MODE_BACKGROUND);
}

/*
** the point corresponds to level adc
*/
void write_val_mark(uint16_t x, uint16_t y, uint32_t size)
{
  static uint16_t old_x;
  static uint16_t old_y;
  fastRect(old_x, old_y, old_x+size, old_y+size, COLOR_BLACK);
  fastRect(x, y, x+size, y+size, COLOR_WHITE);
  old_x = x;
  old_y = y;
}

/*
** the point to position menu
*/
void write_marker_menu_position(uint16_t x, uint16_t y, uint16_t size)
{
  static uint16_t old_x;
  static uint16_t old_y;
  fastRect(old_x, old_y, old_x+size, old_y+size, COLOR_BLACK);
  fastRect(x, y, x+size, y+size, COLOR_WHITE);
  old_x = x;
  old_y = y;
}

/*
** the point corresponds to level adc
*/
void write_marker_rect(uint16_t x, uint16_t y, uint16_t size, uint16_t color)
{
  static uint16_t old_x;
  static uint16_t old_y;
  fastRect(old_x, old_y, old_x+size, old_y+size, COLOR_BLACK);
  fastRect(x, y, x+size, y+size, color);
  old_x = x;
  old_y = y;
}

/*
** drawing status bar
** and menu strings 
*/
void draw_menu_bar(void)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_CYAN;

  tft_fill(1,1,20,tft_W-1,BACK_COLOR); // draw statusbar up
  tft_fill(tft_H-20,1,tft_H-1,tft_W-1,BACK_COLOR); // draw statusbar down

  write_str(offset+xbar*0, ybar, (int8_t*)lb_buffer);
  write_str(offset+xbar*1, ybar, (int8_t*)lb_scrool_y);
  write_str(offset+xbar*2, ybar, (int8_t*)lb_scrool_x);
  write_str(offset+xbar*3, ybar, (int8_t*)lb_speed);
  write_str(offset+xbar*4, ybar, (int8_t*)lb_positive);
  write_str(offset+xbar*5, ybar, (int8_t*)lb_negative);
  write_str(offset+xbar*6, ybar, (int8_t*)lb_gain);
  write_str(offset+xbar*7, ybar, (int8_t*)lb_chanal);
}

/*
** check update scrolling
*/
void menu_upgrade(void)
{
  if(buffer_flag)
  {
    write_val(190,0, buffering);
    #if debug
      write_val_mark(offset+xbar*0, ybar, MARKRECT);
    #endif
    buffer_flag = 0;
  }
  if(scroll_flag_y)
  {
    scroll_flag_y = 0;
    write_val(190,0, buff_scroll_y);
    #if debug
      write_val_mark(offset+xbar*1, ybar, MARKRECT);
    #endif
  }
  if(scroll_flag_x)
  {
    write_val(190,0, buff_scroll_x);
    #if debug
      write_val_mark(offset+xbar*2, ybar, MARKRECT);
    #endif
    scroll_flag_x = 0;
  }
  if(speed_flag)
  {
    write_val(190,0, adc_speed);
    #if debug
      write_val_mark(offset+xbar*3, ybar, MARKRECT);
    #endif
    speed_flag = 0;
  }
  if(trigger_flag_hi)
  {
    write_val(190,0, trigger);
    #if debug
      write_val_mark(offset+xbar*4, ybar, MARKRECT);
    #endif
    trigger_flag_hi = 0;
  }
  if(trigger_flag_lo)
  {
    write_val(190,0, trigger);
    #if debug
      write_val_mark(offset+xbar*5, ybar, MARKRECT);
    #endif
    trigger_flag_lo = 0;
  }
  if(gain_flag)
  {
    write_val(190,0, spi_amplifier_gain(gain));
    spi_amplifier_channel(1);
    #if debug
      write_val_mark(offset+xbar*6, ybar, MARKRECT);
    #endif
    gain_flag = 0;
  }
  if(chanal_flag)
  {
    write_val(190,0, chanal);
    #if debug
      write_val_mark(offset+xbar*7, ybar, MARKRECT);
    #endif
    chanal_flag = 0;
  }
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_GREEN;
}

