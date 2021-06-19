#include "osc.h"

uint16_t BACK_COLOR,POINT_COLOR;
uint32_t adcResult = 0;
uint32_t max_result = 0;
float real_result[480] = {0};

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
 * main loop
 * постоянно вызывается из main
 */
void oscil_run(void)
{
    float t, a1, a2;

    t = 3300/2; // порог в миливольтах
    fill_buff(0); a1 = real_result[0];
    fill_buff(0); a2 = real_result[0];

    while (!((a1 < t) && (a2 >= t))) // т.е. было ниже порога, а стало больше или равно - был переход через порог
    {
      a1 = a2;
      fill_buff(0); a2 = real_result[0];
    }
    
    rotate_buff(5); // scrolling buffer

    for(uint16_t i = 0; i < tft_W; i++) // measurement
    {
      fill_buff(i);
    }

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
  while(adc_counter < 16) // while counter measure less N
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
 * draw string parameters
 */
void write_text(uint16_t i) // for text data
{
  char lbl_real[16] = "mv";
  BACK_COLOR=COLOR_BLUE;
  POINT_COLOR=COLOR_GREEN;
  tft_puts18x32(0,110,(int8_t*)lbl_real,TFT_STRING_MODE_BACKGROUND);

  char real_mv[16];
  utoa((int)real_result[i], real_mv, 10);
  if(real_result[i]<1000)tft_fill(1,62,32,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<100) tft_fill(1,44,32,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<10)  tft_fill(1,26,32,80,BACK_COLOR); //x1 y1 x2 y2
  tft_puts18x32(0,10,(int8_t*)real_mv,TFT_STRING_MODE_BACKGROUND);
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
 * draw grid координат
 */
void write_grid(void)
{
  for(uint16_t x = 32;x<tft_H-31;x+=8)for(uint16_t i = 16; i<tft_W-15; i+=64)tft_draw_point(i, x, COLOR_GRAYBLUE); //draw vertical dot line
  for(uint16_t x = 32;x<tft_H-31;x+=64)for(uint16_t i = 16; i<tft_W-15; i+=8)tft_draw_point(i, x, COLOR_WHITE); //draw horizontal dot line
}
