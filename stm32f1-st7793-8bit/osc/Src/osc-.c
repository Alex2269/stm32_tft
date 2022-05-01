#include "osc.h"
#include "enc.h"
#include "utoa.h"
#include "timers_calback.h"
#include "amplifier.h"

#define SPACE 8

#define GRIDSIZE 8
#define CELLSIZE 48
#define CELLSIZE_HALF (CELLSIZE / 2)
#define BEGIN_H_GRID 4
#define BEGIN_V_GRID (BEGIN_H_GRID*2)
#define WIDTH_GRID (tft_W-8)
#define WIDTH_H_GRID (tft_W-8)
#define WIDTH_V_GRID (tft_W)
#define END_V_GRID (tft_H-20)
#define END_H_GRID (tft_H)

#define WIDTH 400
#define HEIGHT 240

#define BEGIN_RAY 8
#define WIDTH_RAY WIDTH_GRID-8
#define HALF_RAY ((WIDTH_RAY - BEGIN_RAY) / 2) // for fps_count

bool oscil_stop = 0;
uint16_t buffering = 1;

bool chanal_a = 0;
bool chanal_b = 1;

uint16_t ray_dot_line = 1; // ray lines = 1 or points only = 0

uint32_t adc_tmp_a = 0;
uint32_t adc_tmp_b = 0;

uint32_t adcResult_a = 0;
uint32_t adcResult_b = 0;

uint32_t max_result_a = 0;
uint32_t max_result_b = 0;

uint16_t trigger = 0; // порог в миливольтах
uint16_t buff_scroll_x = 0;
int16_t buff_scroll_y = 24;
uint16_t buff_trigger = 0;

uint32_t adc_speed = 1;
uint32_t adc_counter = 0;
uint16_t fps_count = 0;

float real_result_a[WIDTH] = {0};
float real_result_b[WIDTH] = {0};

uint16_t buff_clean_a[WIDTH] = {0};
uint16_t *ptr_clean_a = buff_clean_a;
uint16_t buff_ray_a[WIDTH] = {0};
uint16_t *ptr_ray_a = buff_ray_a;

uint16_t buff_clean_b[WIDTH] = {0};
uint16_t *ptr_clean_b = buff_clean_b;
uint16_t buff_ray_b[WIDTH] = {0};
uint16_t *ptr_ray_b = buff_ray_b;

uint16_t border_y_min = 24; // границы размаха луча
uint16_t border_y_max = 216; // границы размаха луча

/*
** init
** begin draw coordinate grid
** and string parameters(write_text)
*/
void oscil_init(void)
{
  spi_amplifier_gain(1);
  spi_amplifier_channel(1);
  setRotation(1);
  fastRect(0, 0, tft_W, tft_H, 0);
  tft_clear(COLOR_BLACK);
  write_grid(); // draw coordinate grid
  draw_menu_bar(); // drawing status bar, and menu bar
}

/*
** main loop
** постоянно вызывается из main
*/
void oscil_run(void)
{
  run_buffering();
  /* if(buffering && (adc_speed < 32))
  {
    run_buffering();
  }
  else
  {
    run_no_buffering();
  } */
}

/*
** синхронизация включена
** trigger capture enable
*/
void trigger_on(uint16_t trig)
{
  uint16_t a1, a2;
  uint16_t count = 0;

  fill_buff(0); a1 = real_result_a[0];
  fill_buff(0); a2 = real_result_a[0];
  
  while (!((a1 > trig) && (a2 <= trig))) // т.е. было выше порога, а стало ниже или равно - был переход через порог
  {
    count++;
    a1 = a2;
    fill_buff(0); a2 = real_result_a[0];
    if(count>800)break; // if loopping
  }

  for(uint16_t i = BEGIN_RAY; i < WIDTH_RAY; i++) // measurement
  {
    fill_buff(i+buff_scroll_x);
  }
}

/*
** синхронизация выключена
** trigger capture disable
*/
void trigger_off(void)
{
  for(uint16_t i = BEGIN_RAY; i < WIDTH_RAY; i++) // measurement
  {
    fill_buff(i);
  }
}

/*
** mode buffering
*/
void run_buffering(void)
{
  if(oscil_stop) return;
  //
  if(trigger>500 && trigger<2800)
  {
    trigger_on(trigger);
  }
  else
  {
    trigger_off();
  }

  // rotate_buff(buff_scroll_x); // scrolling buffer

  for(uint16_t i = BEGIN_RAY; i < WIDTH_RAY; i++) // display redraw
  {
    graph_ray_clean_a(i); // clean old ray
    graph_ray_clean_b(i); // clean old ray
    tic_delay(1); // nothing means
    graph_ray_draw_a(i); // draw new ray
    graph_ray_draw_b(i); // draw new ray

    if(i == HALF_RAY) // show fps
    {
      fps_count++;
    }
  }
  // --
  write_grid(); // upgrade grid
}

/*
** mode without buffering
*/
void run_no_buffering(void)
{
  if(oscil_stop) return;
  rotate_buff(buff_scroll_x); // scrolling buffer
  for(uint16_t i = BEGIN_RAY; i < WIDTH_RAY; i++) // display redraw
  {
    // if(!i)tft_fill_grid(24/*y1*/, 0/*x1*/, 240-24/*y2*/, tft_W/*x2*/, COLOR_BLACK);
    for(uint32_t delay = 0; delay < adc_speed; delay ++)
    {
      tic_delay(1);
    }
    // --
    if(!ray_dot_line && i)
    {
      tft_draw_dot_line(i+0, *(ptr_ray_a+i+0), i+1, *(ptr_ray_a+i+1), COLOR_BLACK); // draw old ray;
      tft_draw_dot_line(i+0, *(ptr_ray_b+i+0), i+1, *(ptr_ray_b+i+1), COLOR_BLACK); // draw old ray;

      fill_buff(i);
      tft_draw_dot_line(i-1, *(ptr_ray_a+i-1), i+0, *(ptr_ray_a +i), COLOR_YELLOW); // draw new ray;
      tft_draw_dot_line(i-1, *(ptr_ray_b+i-1), i+0, *(ptr_ray_b +i), COLOR_GREEN); // draw new ray;
    }
    else if(ray_dot_line && i)
    {
      if(i > BEGIN_RAY && i < WIDTH_RAY)
      {
        fill_buff(i);
        graph_ray_clean_a(i); // clean old ray
        graph_ray_clean_b(i); // clean old ray
      }
      // --
      tic_delay(1); // nothing means
      if(i > BEGIN_RAY && i < WIDTH_RAY)
      {
        graph_ray_draw_a(i-2); // draw new ray
        graph_ray_draw_b(i-2); // draw new ray
      }
      // --
    }
    // --
    if(i == HALF_RAY) // show fps
    {
      fps_count++;
    }
    // --
  }
  // --
  write_grid(); // upgrade grid
}

/*
** save measure adc and fill buffers buff_ray[i] & real_result[i]
** buff_ray[] use for draw ray with ofset vertical
** real_result[] save measure from adc with coefitient for move volts to bars
*/
void fill_buff(uint16_t i)
{
  if(i>=WIDTH_RAY)i-=WIDTH_RAY;

  while(adc_counter < adc_speed) // while counter measure less N
  {
   /*
    * нужно перевернуть  значение ADC, так как координаты дисплея считаются с верхнего левого угла,
    * нужно отрисовывать луч так, чтобы внизу было меньшее значение ADC
    */
    adc_tmp_a = HAL_ADC_GetValue(&hadc1); // Сумируем полученные значения ацп
    adcResult_a += (uint16_t)(adc_tmp_a>>0);
    adc_tmp_b = HAL_ADC_GetValue(&hadc2); // Сумируем полученные значения ацп
    adcResult_b += (uint16_t)(adc_tmp_b>>0);
    // --
    adc_counter++; // Увеличиваем счетчик измерений
  }

  buff_ray_a[i] = tft_H - buff_scroll_y - adcResult_a / adc_counter / 20; // Вычисляем среднее значение
  if(buff_ray_a[i] > (tft_H-24))buff_ray_a[i] = (tft_H-24);
  if(buff_ray_a[i] < 24)buff_ray_a[i] = 24;
  real_result_a[i] = adcResult_a / adc_counter * 0.82; // 3300/4096=0.80566 Вычисляем среднее значение

  buff_ray_b[i] = tft_H - buff_scroll_y - adcResult_b / adc_counter / 20; // Вычисляем среднее значение
  if(buff_ray_b[i] > (tft_H-24))buff_ray_b[i] = (tft_H-24);
  if(buff_ray_b[i] < 24)buff_ray_b[i] = 24;
  real_result_b[i] = adcResult_b / adc_counter * 0.82; // 3300/4096=0.80566 Вычисляем среднее значение

  // if(real_result_a[i]<500) real_result_a[i] = 0;
  // if(real_result_a[i]>500) real_result_a[i] -= 500;
  // real_result_a[i] *= .4; // значение в барах

  // if(real_result_b[i]<500) real_result_b[i] = 0;
  // if(real_result_b[i]>500) real_result_b[i] -= 500;
  // real_result_b[i] *= .4; // значение в барах

  adcResult_a = 0; // Обнуляем значение
  adcResult_b = 0; // Обнуляем значение
  adc_counter = 0; // Обнуляем счетчик измерений

  // if(real_result_a[i]>max_result_a) max_result_a = real_result_a[i]; // save max value
  // if(real_result_b[i]>max_result_b) max_result_b = real_result_b[i]; // save max value
}

void rotate_buff(uint16_t r)
{
  uint16_t tmp;
  for(uint16_t i = 0; i < r; i++) // scroll buff_ray_a
  {
    tmp = buff_ray_a[WIDTH_RAY-1];
    for(uint16_t i = WIDTH_RAY-1; i; i--)
    {
      buff_ray_a[i] = buff_ray_a[i-1];
    }
    buff_ray_a[0] = tmp;
  }
}

/*
** flush buffer
** очистка буфера
*/
void flush_buff(void)
{
  for(uint16_t i = BEGIN_RAY; i < WIDTH_RAY; i++) // measurement
  {
    real_result_a[i] = 0;
    real_result_b[i] = 0;

    buff_clean_a[i] = 0;
    buff_ray_a[i] = 0;

    buff_clean_b[i] = 0;
    buff_ray_b[i] = 0;
  }
}

/*
** draw old ray
*/
void graph_ray_clean_a(uint16_t i)
{
  if(i>=WIDTH_RAY)i-=WIDTH_RAY;

  tft_draw_dot_line(i+1,  *(ptr_clean_a+i), i+2, *(ptr_clean_a+i+1), COLOR_BLACK); //draw old ray;
}
/*
 * draw old ray
 */
void graph_ray_clean_b(uint16_t i)
{
  if(i>=WIDTH_RAY)i-=WIDTH_RAY;

  tft_draw_dot_line(i+1,  *(ptr_clean_b+i), i+2, *(ptr_clean_b+i+1), COLOR_BLACK); //draw old ray;
}

/*
** draw new ray
*/
void graph_ray_draw_a(uint16_t i)
{
  if(*(ptr_ray_a+i)<border_y_min) *(ptr_ray_a+i)=border_y_min; // range borders
  if(*(ptr_ray_a+i+1)<border_y_min) *(ptr_ray_a+i+1)=border_y_min; // range borders
  if(*(ptr_ray_a+i)>border_y_max) *(ptr_ray_a+i)=border_y_max; // range borders
  if(*(ptr_ray_a+i+1)>border_y_max) *(ptr_ray_a+i+1)=border_y_max; // range borders

  // if(*(ptr_ray_a+i)<border_y_min)return;// *(ptr_ray_a+i)=border_y_min; // range borders
  // if(*(ptr_ray_a+i+1)<border_y_min)return;// *(ptr_ray_a+i+1)=border_y_min; // range borders

  // if(*(ptr_ray_a+i)>border_y_max)return;// *(ptr_ray_a+i)=border_y_max; // range borders
  // if(*(ptr_ray_a+i+1)>border_y_max)return;// *(ptr_ray_a+i+1)=border_y_max; // range borders

  if(i>=WIDTH_RAY)i-=WIDTH_RAY;

  if(i>BEGIN_RAY && i < WIDTH_RAY-1) // draw new ray
  {
    tft_draw_dot_line(i+0, *(ptr_ray_a  +i), i+1, *(ptr_ray_a  +i+1), COLOR_YELLOW); //draw new ray;
  }

  *(ptr_clean_a+i) = *(ptr_ray_a+i+1);
}

/*
** draw new ray
*/
void graph_ray_draw_b(uint16_t i)
{
  if(*(ptr_ray_b+i)<border_y_min) *(ptr_ray_b+i)=border_y_min; // range borders
  if(*(ptr_ray_b+i+1)<border_y_min) *(ptr_ray_b+i+1)=border_y_min; // range borders
  if(*(ptr_ray_b+i)>border_y_max) *(ptr_ray_b+i)=border_y_max; // range borders
  if(*(ptr_ray_b+i+1)>border_y_max) *(ptr_ray_b+i+1)=border_y_max; // range borders

  // if(*(ptr_ray_b+i)<border_y_min)return;// *(ptr_ray_b+i)=border_y_min; // range borders
  // if(*(ptr_ray_b+i+1)<border_y_min)return;// *(ptr_ray_b+i+1)=border_y_min; // range borders

  // if(*(ptr_ray_b+i)>border_y_max)return;// *(ptr_ray_b+i)=border_y_max; // range borders
  // if(*(ptr_ray_b+i+1)>border_y_max)return;// *(ptr_ray_b+i+1)=border_y_max; // range borders

  if(i>=WIDTH_RAY)i-=WIDTH_RAY;

  if(i>BEGIN_RAY && i < WIDTH_RAY-1) // draw new ray
  {
    tft_draw_dot_line(i+0, *(ptr_ray_b  +i), i+1, *(ptr_ray_b  +i+1), COLOR_GREEN); //draw new ray;
  }

  *(ptr_clean_b+i) = *(ptr_ray_b+i+1);
}

/*
** draw grid
** координатная сетка
*/
void write_grid(void)
{
  for(uint16_t x = CELLSIZE_HALF;x<END_V_GRID;x+=GRIDSIZE)for(uint16_t i = BEGIN_V_GRID<<0; i<WIDTH_V_GRID; i+=CELLSIZE)tft_draw_point(i, x, COLOR_CYAN); // draw vertical dot line
  for(uint16_t x = CELLSIZE_HALF;x<END_H_GRID;x+=CELLSIZE)for(uint16_t i = BEGIN_H_GRID<<1; i<WIDTH_H_GRID; i+=GRIDSIZE)tft_draw_point(i, x, COLOR_WHITE); // draw horizontal dot line
}

