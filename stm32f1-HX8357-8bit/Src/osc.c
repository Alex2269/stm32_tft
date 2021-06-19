#include "osc.h"
#include "enc.h"
#include "generator.h"

bool oscil_stop = 0;
uint16_t BACK_COLOR,POINT_COLOR;
uint32_t adcResult = 0;
uint32_t max_result = 0;
float real_result[480] = {0};
uint16_t trigger = 1100; // порог в миливольтах

uint16_t buff_scroll = 0;
uint16_t buff_trigger = 0;
uint16_t adc_speed = 1;

uint16_t adc_counter = 0;

uint16_t i = 0;

uint16_t buff_clean[480] = {0};
//uint16_t coord_x[480] = {0};
uint16_t buff_ray[480] = {0};

/*
 * init
 * begin draw grid
 * и string parameters(write_text)
 */
void oscil_init(void)
{
  tft_SetRotation(1);
  tft_clear(COLOR_BLACK);

  tft_draw_rectangle(31,0,0,tft_W,COLOR_RED); // draw contour statusbar
  tft_fill(1,1,30,tft_W-1,COLOR_YELLOW); // draw statusbar

  tft_draw_rectangle(tft_H-31,0,tft_H,tft_W,COLOR_RED); // draw contour statusbar
  tft_fill(tft_H-30,1,tft_H-1,tft_W-1,COLOR_YELLOW); // draw statusbar

  write_grid(); // draw grid
  write_text(i); // draw label & voltage measurement

  BACK_COLOR=COLOR_RED;
  POINT_COLOR=COLOR_BLUE;
  tft_putchar(tft_W/2,20,'A',TFT_STRING_MODE_NO_BACKGROUND);
}

/*
 * синхронизация включена
 * trigger capture enable
 */
void trigger_on(uint16_t trig)
{
  uint16_t a1, a2;
  uint16_t count = 0;

  fill_buff(0); a1 = real_result[0];
  fill_buff(0); a2 = real_result[0];
  
  while (!((a1 < trig) && (a2 >= trig))) // т.е. было ниже порога, а стало больше или равно - был переход через порог
  {
    count++;
    a1 = a2;
    fill_buff(0); a2 = real_result[0];
    if(count>800)break; // if loopping
  }

  for(uint16_t i = 0; i < tft_W; i++) // measurement
  {
    fill_buff(i);
  }
}

/*
 * синхронизация выключена
 * trigger capture disable
 */
void trigger_off(void)
{
  for(uint16_t i = 0; i < tft_W; i++) // measurement
  {
    fill_buff(i);
  }
}

/*
 * main loop
 * постоянно вызывается из main
 */
void oscil_run(void)
{
  if(oscil_stop) return;

  trigger_on(trigger);

  /* if(trigger>500 && trigger<2500)
  {
    trigger_on(trigger);
  }
  else
  {
    trigger_off();
  } */

  rotate_buff(buff_scroll); // scrolling buffer

  for(uint16_t i = 0; i < tft_W; i++) // display redraw
  {
    write_graph(i);
  }

  write_grid(); // upgrade grid
  write_text(i); // upgrade label & voltage measurement
}

/*
 * save measure adc and fill buffers buff_ray[i] & real_result[i]
 * buff_ray[] use for draw ray with ofset vertical
 * real_result[] save measure from adc with coefitient for move volts to bars
 */
void fill_buff(uint16_t i)
{
  while(adc_counter < adc_speed) // while counter measure less N
  {
    adcResult += HAL_ADC_GetValue(&hadc1); // Сумируем полученные значения ацп
    adc_counter++; // Увеличиваем счетчик измерений
  }

  buff_ray[i] = tft_H-45-adcResult/adc_counter/18; // Вычисляем среднее значение
  real_result[i] = adcResult/adc_counter*0.82; // 3300/4096=0.80566 Вычисляем среднее значение

  // if(real_result[i]<500) real_result[i] = 0;
  // if(real_result[i]>500) real_result[i] -= 500;
  // real_result[i] *= .4; // значение в барах

  adcResult = 0; // Обнуляем значение
  adc_counter = 0; // Обнуляем счетчик измерений
  if(real_result[i]>max_result) max_result = real_result[i]; // save max value
}

void rotate_buff(uint16_t r)
{
    uint16_t tmp;
    for(i = 0; i < r; i++) // scroll buff_ray
    {
      tmp = buff_ray[tft_W-1];
      for(uint16_t i = tft_W-1; i; i--)
      {
        buff_ray[i] = buff_ray[i-1];
      }
      buff_ray[0] = tmp;
    }
}

/*
 * draw old ray
 */
void clean_ray(void)
{
  for(uint16_t i = 0; i < tft_W; i++)
  {
    tft_draw_dot_line(i+1, buff_clean[i+1], i, buff_clean[i], COLOR_BLACK); //draw old ray;
  }
}

/*
 * flush buffer
 * очистка буфера
 */
void flush_buff(void)
{
  for(uint16_t i = 0; i < tft_W; i++) // measurement
  {
    real_result[i] = 0;
    buff_clean[i] = 0;
    buff_ray[i] = 0;
  }
}

/*
 * the point corresponds to level adc
 */
void write_marker_point(uint16_t x, uint16_t y, uint16_t size)
{
  static uint16_t old_x;
  static uint16_t old_y;
  tft_draw_point_big(old_x, old_y, size, COLOR_BLACK); // clear marker points
  tft_draw_point_big(x, y, size, COLOR_WHITE); // draw marker point
  old_x = x;
  old_y = y;
}

/*
 * draw string parameters
 */
void write_text(uint16_t i) // for text data
{
  char lbl_real[16] = "mv";
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_GREEN;
  tft_puts14x24(0,90,(int8_t*)lbl_real,TFT_STRING_MODE_BACKGROUND);

  char real_mv[16];
  utoa((int)real_result[i], real_mv, 10); // sprintf(real_mv,"%d",real_result[i]);
  if(real_result[i]<1000)tft_fill(1,50,24,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<100) tft_fill(1,36,24,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<10)  tft_fill(1,22,24,80,BACK_COLOR); //x1 y1 x2 y2
  tft_puts14x24(0,10,(int8_t*)real_mv,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw string
 */
void write_string(uint16_t x, uint16_t y, int8_t* label)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_GREEN;
  tft_puts14x24(y,x,label,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw parameters
 */
void write_value(uint16_t x, uint16_t y, uint16_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_GREEN;

  char speed_value[16];
  utoa((int)value, speed_value, 10); // sprintf(speed_value,"%d",value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<10000) tft_fill(y,x+12*4,y+24,x+14*5,BACK_COLOR); //x1 y1 x2 y2
  if(value<1000)  tft_fill(y,x+12*3,y+24,x+14*5,BACK_COLOR); //x1 y1 x2 y2
  if(value<100)   tft_fill(y,x+12*2,y+24,x+14*5,BACK_COLOR); //x1 y1 x2 y2
  if(value<10)    tft_fill(y,x+12*1,y+24,x+14*5,BACK_COLOR); //x1 y1 x2 y2
  tft_puts14x24(y,x,(int8_t*)speed_value,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw old ray & draw new ray
 */
void write_graph(uint16_t i)
{
  //write_text(i);
  if(i>0 && i < tft_W-0){ // clean old ray
    //tft_draw_point_big(i, buff_clean[i],COLOR_BLACK); //draw old ray;
    //tft_draw_line_slim(i+1, buff_clean[i+1], i, buff_clean[i], COLOR_BLACK); //draw old ray;
    tft_draw_dot_line(i+1, buff_clean[i+1], i, buff_clean[i], COLOR_BLACK); //draw old ray;
  }
  if(i>2 && i < tft_W-2){ // draw new ray
    //tft_draw_point_big(i, buff_ray[i], COLOR_WHITE); //draw new ray;
    //tft_draw_line_slim(i, buff_ray[i], i-1, buff_ray[i-1], COLOR_WHITE); //draw new ray;
    tft_draw_dot_line(i, buff_ray[i], i-1, buff_ray[i-1], COLOR_WHITE); //draw new ray;
  }
  buff_clean[i]=buff_ray[i];
}

/*
 * draw grid
 * координатная сетка
 */
void write_grid(void)
{
  for(uint16_t x = 32;x<tft_H-31;x+=8)for(uint16_t i = 16; i<tft_W-15; i+=64)tft_draw_point(i, x, COLOR_GRAYBLUE); //draw vertical dot line
  for(uint16_t x = 32;x<tft_H-31;x+=64)for(uint16_t i = 16; i<tft_W-15; i+=8)tft_draw_point(i, x, COLOR_WHITE); //draw horizontal dot line
}
