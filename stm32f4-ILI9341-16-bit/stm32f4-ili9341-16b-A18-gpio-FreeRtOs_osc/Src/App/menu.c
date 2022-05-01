#include "osc.h"
#include "menu.h"

#include "math.h"
#include "Red_Button.h"
#include "Green_button.h"
#include "Yellow_Button.h"

// convert button.jpg -define bmp:subtype=RGB565 button.bmp
// mogrify -resize "64x64"! button.jpg

#define TOUCH_TEST 0
#define debug 0

extern TS_Point p; // define on osc.c

volatile bool ts_top_menu;
volatile bool ts_red_menu;
volatile bool ts_yellow_menu;
volatile bool ts_pressed;
volatile uint32_t ts_divide;

push_button pb_dv_20;
push_button pb_dv_50;
push_button pb_dv_100;
push_button pb_dv_200;

push_button pb_dv_500;
push_button pb_dv_1000;
push_button pb_dv_2000;
push_button pb_dv_5000;

push_button pb_dv_10000;
push_button pb_dv_20000;
push_button pb_dv_50000;
push_button pb_dv_100000;

void menu_init(void)
{
#ifdef debug
  get_point(&p); // get coordinates x y z from xpt2046
  // get_touch(&p); // get coordinates x y z from xpt2046
  write_info_touch();
#endif
  // static volatile uint32_t s;
  // s = get_time_sec();

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(4, 4, 24, 24); // clear button

  // if(pbut_hello->Xpos > (pbut_hello->Xpos-40) && pbut_hello->Xpos<(pbut_hello->Xpos+40) && pbut_hello->Ypos > (pbut_hello->Ypos-20) && pbut_hello->Ypos<(pbut_hello->Ypos+20))
  if(p.x>20 && p.x<100 && p.y > 45 && p.y < 90)
  {
    ts_top_menu = 0;
    ts_red_menu = 0;
    ts_pressed = 0;
    ts_yellow_menu = 1;

    BSP_LCD_Clear(LCD_COLOR_BLACK);
  }

  if(p.x>140 && p.x<300 && p.y > 45 && p.y < 90)
  {
    ts_top_menu = 0;
    ts_red_menu = 1;
    ts_pressed = 0;
  ts_yellow_menu = 0;

    BSP_LCD_Clear(LCD_COLOR_BLACK);
  }

  if(ts_top_menu)
  {
    ts_pressed = 0;
    HAL_Delay(300);
    // BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    // BSP_LCD_FillRect(24, 54, 64, 36); // draw button
    if(p.x>20 && p.x<94 && p.y > 120 && p.y < 155) // artivate menu
    {
      //BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      //BSP_LCD_FillRect(4, 4, 24, 36); // clear button
      // ts_pressed = 1;
    }

    BSP_LCD_DrawRGB16Image(22, 34,   120, 72, (uint16_t*)Green_button);
    BSP_LCD_DrawRGB16Image(22, 130,  120, 72, (uint16_t*)Green_button);
    BSP_LCD_DrawRGB16Image(174, 34,  120, 72, (uint16_t*)Green_button);
    BSP_LCD_DrawRGB16Image(174, 130, 120, 72, (uint16_t*)Green_button);

    BSP_LCD_DisplayStringAt(36, 42, (uint8_t *)" OFF ", LEFT_MODE);

    // BSP_LCD_DrawBitmap(28, 30, (uint16_t *)buttonArray);
    // BSP_LCD_DrawRGB16Image(122,22, 40, 32, (uint16_t *)pixel_data);
  }

  if(ts_red_menu)
  {
    ts_top_menu = 0;
    // ts_red_menu = 0;
    ts_pressed = 0;
    ts_yellow_menu = 0;

    call_button_red();
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    ts_red_menu = 0;
  }

  if(ts_yellow_menu)
  {
    ts_top_menu = 0;
    ts_red_menu = 0;
    ts_pressed = 0;
    // ts_yellow_menu = 0;

    call_button_yell();
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    ts_yellow_menu = 0;
  }

  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
}

void write_info_touch(void)
{
  if(!TOUCH_TEST) return;
  static volatile uint16_t count;
  count++;
  if(count<5) return;
  count = 0;

  char txt[60];
  snprintf(txt, 60, "%d X %d Y %d Z", (unsigned int)p.x, (unsigned int)p.y, (unsigned int)p.z);

#ifdef TOUCH_TEST
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(4, 4, (uint8_t *)txt, LEFT_MODE);
#endif
}

void call_button_yell(void)
{
    draw_button(&pb_dv_20, (uint16_t*) Yellow_Button, (uint8_t *)"1",       22, 10,   80, 40,  LCD_COLOR_YELLOW, (uint8_t *)" 20us");
    draw_button(&pb_dv_50, (uint16_t*) Yellow_Button, (uint8_t *)"2",       22, 70,   80, 40,  LCD_COLOR_YELLOW, (uint8_t *)" 50us");
    draw_button(&pb_dv_100, (uint16_t*) Yellow_Button, (uint8_t *)"3",      22, 130,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"0.1ms");
    draw_button(&pb_dv_200, (uint16_t*) Yellow_Button, (uint8_t *)"4",      22, 190,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"0.2ms");
    draw_button(&pb_dv_500, (uint16_t*) Yellow_Button, (uint8_t *)"5",      120, 10,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"0.5ms");
    draw_button(&pb_dv_1000, (uint16_t*) Yellow_Button, (uint8_t *)"6",     120, 70,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"  1ms");
    draw_button(&pb_dv_2000, (uint16_t*) Yellow_Button, (uint8_t *)"7",     120, 130, 80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"  2ms");
    draw_button(&pb_dv_5000, (uint16_t*) Yellow_Button, (uint8_t *)"8",     120, 190, 80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"  5ms");
    draw_button(&pb_dv_10000, (uint16_t*) Yellow_Button, (uint8_t *)"9",    220, 10,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)" 10ms");
    draw_button(&pb_dv_20000, (uint16_t*) Yellow_Button, (uint8_t *)"10",   220, 70,  80, 40,  LCD_COLOR_YELLOW, (uint8_t *)" 20ms");
    draw_button(&pb_dv_50000, (uint16_t*) Yellow_Button, (uint8_t *)"11",   220, 130, 80, 40,  LCD_COLOR_YELLOW, (uint8_t *)" 50ms");
    draw_button(&pb_dv_100000, (uint16_t*) Yellow_Button, (uint8_t *)"12",  220, 190, 80, 40,  LCD_COLOR_YELLOW, (uint8_t *)"100ms");
    HAL_Delay(1900); // freezing for visible button
    
    // get_point(&p); // get coordinates x y z from xpt2046
    if(get_button(&pb_dv_20)) { ts_divide=20; }
    if(get_button(&pb_dv_50)) { ts_divide=50; }
    if(get_button(&pb_dv_100)) { ts_divide=100; }
    if(get_button(&pb_dv_200)) { ts_divide=200; }
    if(get_button(&pb_dv_500)) { ts_divide=500; }
    if(get_button(&pb_dv_1000)) { ts_divide=1000; }
    if(get_button(&pb_dv_2000)) { ts_divide=2000; }
    if(get_button(&pb_dv_5000)) { ts_divide=5000; }
    if(get_button(&pb_dv_10000)) { ts_divide=10000; }
    if(get_button(&pb_dv_20000)) { ts_divide=20000; }
    if(get_button(&pb_dv_50000)) { ts_divide=50000; }
    if(get_button(&pb_dv_100000)) { ts_divide=100000; }
}

void call_button_red(void)
{
    draw_button(&pb_dv_20, (uint16_t*) Red_Button, (uint8_t *)"1",       22, 10,   80, 40,  LCD_COLOR_RED, (uint8_t *)" 20us");
    draw_button(&pb_dv_50, (uint16_t*) Red_Button, (uint8_t *)"2",       22, 70,   80, 40,  LCD_COLOR_RED, (uint8_t *)" 50us");
    draw_button(&pb_dv_100, (uint16_t*) Red_Button, (uint8_t *)"3",      22, 130,  80, 40,  LCD_COLOR_RED, (uint8_t *)"0.1ms");
    draw_button(&pb_dv_200, (uint16_t*) Red_Button, (uint8_t *)"4",      22, 190,  80, 40,  LCD_COLOR_RED, (uint8_t *)"0.2ms");
    draw_button(&pb_dv_500, (uint16_t*) Red_Button, (uint8_t *)"5",      120, 10,  80, 40,  LCD_COLOR_RED, (uint8_t *)"0.5ms");
    draw_button(&pb_dv_1000, (uint16_t*) Red_Button, (uint8_t *)"6",     120, 70,  80, 40,  LCD_COLOR_RED, (uint8_t *)"  1ms");
    draw_button(&pb_dv_2000, (uint16_t*) Red_Button, (uint8_t *)"7",     120, 130, 80, 40,  LCD_COLOR_RED, (uint8_t *)"  2ms");
    draw_button(&pb_dv_5000, (uint16_t*) Red_Button, (uint8_t *)"8",     120, 190, 80, 40,  LCD_COLOR_RED, (uint8_t *)"  5ms");
    draw_button(&pb_dv_10000, (uint16_t*) Red_Button, (uint8_t *)"9",    220, 10,  80, 40,  LCD_COLOR_RED, (uint8_t *)" 10ms");
    draw_button(&pb_dv_20000, (uint16_t*) Red_Button, (uint8_t *)"10",   220, 70,  80, 40,  LCD_COLOR_RED, (uint8_t *)" 20ms");
    draw_button(&pb_dv_50000, (uint16_t*) Red_Button, (uint8_t *)"11",   220, 130, 80, 40,  LCD_COLOR_RED, (uint8_t *)" 50ms");
    draw_button(&pb_dv_100000, (uint16_t*) Red_Button, (uint8_t *)"12",  220, 190, 80, 40,  LCD_COLOR_RED, (uint8_t *)"100ms");
    HAL_Delay(1900); // freezing for visible button
    
    // get_point(&p); // get coordinates x y z from xpt2046
    if(get_button(&pb_dv_20)) { ts_divide=20; }
    if(get_button(&pb_dv_50)) { ts_divide=50; }
    if(get_button(&pb_dv_100)) { ts_divide=100; }
    if(get_button(&pb_dv_200)) { ts_divide=200; }
    if(get_button(&pb_dv_500)) { ts_divide=500; }
    if(get_button(&pb_dv_1000)) { ts_divide=1000; }
    if(get_button(&pb_dv_2000)) { ts_divide=2000; }
    if(get_button(&pb_dv_5000)) { ts_divide=5000; }
    if(get_button(&pb_dv_10000)) { ts_divide=10000; }
    if(get_button(&pb_dv_20000)) { ts_divide=20000; }
    if(get_button(&pb_dv_50000)) { ts_divide=50000; }
    if(get_button(&pb_dv_100000)) { ts_divide=100000; }
}


void draw_button(push_button* pb, uint16_t* image, uint8_t* name, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color, uint8_t* text)
{
  BSP_LCD_SetTextColor(~Color);
  BSP_LCD_SetBackColor(Color);

  // BSP_LCD_DrawRect(Xpos-2, Ypos-2, Width+4, Height+4);
  // BSP_LCD_SetTextColor(Color);
  // BSP_LCD_FillRect(Xpos+2, Ypos+2, Width-4, Height-4);
  BSP_LCD_DrawRGB16Image(Xpos, Ypos, 80, 40, image);

  pb->X=Xpos+(Width>>1);
  pb->Y=Ypos+(Height>>1);

  pb->W=Width;
  pb->H=Height;

  pb->Color=Color;
  pb->text=text;

  pb->active = 0;

  BSP_LCD_DisplayStringAt(Xpos+4, Ypos+(Height>>2), pb->text, LEFT_MODE);
}

uint8_t get_button(push_button* pb)
{
  get_point(&p); // get coordinates x y z from xpt2046
  if( p.x > (pb->X) &&
      p.x < (pb->X+(pb->W)) &&
      p.y > (pb->Y) &&
      p.y < (pb->Y+(pb->H)) )
  {
    pb->active=1;
  }
  else
  { 
    pb->active = 0;
  }

  char txt[60];
  snprintf(txt, 60, "%d : %d : %s : %d", (uint16_t)p.x, (uint16_t)p.y, pb->name, pb->active);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  // if(pb->active) BSP_LCD_DisplayStringAt(4, BSP_LCD_GetYSize()-24, (uint8_t *)txt, LEFT_MODE);

  return pb->active;
}

