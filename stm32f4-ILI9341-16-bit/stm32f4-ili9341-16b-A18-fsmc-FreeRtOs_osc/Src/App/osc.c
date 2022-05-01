#include "osc.h"
#include "menu.h"

#ifdef  osCMSIS
#define Delay(t)              osDelay(t)
#define GetTime()             osKernelSysTick()
#else
#define Delay(t)              HAL_Delay(t)
#define GetTime()             HAL_GetTick()
#endif

volatile uint16_t graph_pos;

TS_Point p;

uint64_t millis;

uint32_t adcResult_ch1 = 0;
uint32_t adcResult_ch2 = 0;
uint32_t adcResult_ch3 = 0;
uint32_t adcResult_ch4 = 0;

uint32_t max_result_ch1 = 0;
uint32_t max_result_ch2 = 0;
uint32_t max_result_ch3 = 0;
uint32_t max_result_ch4 = 0;

uint16_t buff_ch1[480] = {0};
uint16_t buff_ch2[480] = {0};
uint16_t buff_ch3[480] = {0};
uint16_t buff_ch4[480] = {0};

float real_result_ch1[480] = {0};
float real_result_ch2[480] = {0};
float real_result_ch3[480] = {0};
float real_result_ch4[480] = {0};

uint16_t buff_clean_ch1[480] = {0};
uint16_t buff_clean_ch2[480] = {0};
uint16_t buff_clean_ch3[480] = {0};
uint16_t buff_clean_ch4[480] = {0};

uint16_t buff_ray_ch1[480] = {0};
uint16_t buff_ray_ch2[480] = {0};
uint16_t buff_ray_ch3[480] = {0};
uint16_t buff_ray_ch4[480] = {0};

#define ADC_CH 4
#define ADC_BUF_SIZE 320*ADC_CH

volatile uint16_t ADC_Data[ADC_BUF_SIZE];

uint16_t i = 0;

void write_info_top(uint32_t ms)
{
  static volatile uint16_t count;
  count++;
  if(count<5) return;
  count = 0;

  char txt[30];
  snprintf(txt, 30, "%d ms   %d fps ", (unsigned int)ms, 1000 / (unsigned int)ms);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(80, 6, (uint8_t *)txt, LEFT_MODE);
}

void write_info_down(uint32_t us)
{
  static volatile uint16_t count;
  count++;
  if(count<5) return;
  count = 0;

  char txt[30];
  snprintf(txt, 30, "%d us/Div   %d,%d Hz ", (unsigned int)us/10, 10000000 / (unsigned int)us,(10000000 / (unsigned int)us)%10);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(4, BSP_LCD_GetYSize()-24, (uint8_t *)txt, LEFT_MODE);
}

/*
 * init
 * begin draw grid
 * и string parameters(write_text)
 */
void oscil_init(void)
{
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &ADC_Data,ADC_BUF_SIZE);
  // HAL_ADC_Start_IT(&hadc1);

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  // penguin, pinguin, raccon

  // BSP_LCD_DrawRGB16Image(16, 54, 80, 36, (uint16_t*)Red_Button);
  // BSP_LCD_DrawBitmap(28, 30, (uint16_t *)buttonArray);
  // BSP_LCD_DrawRGB16Image(122,22, 40, 32, (uint16_t *)pixel_data);

  Delay(1000);

  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawRect(0, 2, BSP_LCD_GetXSize()-1, 26); // draw contour statusbar up
  BSP_LCD_DrawRect(0, BSP_LCD_GetYSize()-28, BSP_LCD_GetXSize()-4, 26); // draw contour statusbar down
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillRect(2, 4, BSP_LCD_GetXSize()-4, 22); // draw statusbar up
  BSP_LCD_FillRect(2, BSP_LCD_GetYSize()-26, BSP_LCD_GetXSize()-4, 22); // draw statusbar down

  write_grid(); // draw grid
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_SetBackColor(LCD_COLOR_CYAN);
  BSP_LCD_SetFont(&Font20);
}

uint32_t ClearTest(void)
{
  uint32_t ctStartT = GetTime();
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  return(GetTime() - ctStartT);
}

extern volatile bool ts_top_menu;
extern volatile bool ts_red_menu;
extern volatile bool ts_yellow_menu;
extern volatile bool ts_pressed;
extern volatile uint32_t ts_divide;

/*
 * main loop
 * постоянно вызывается из main
 */
void oscil_run(void)
{
    if (ts_pressed || ts_top_menu || ts_red_menu || ts_yellow_menu) return;
    // get_point(&p); // get coordinates x y z from xpt2046
    // get_touch(&p); // get coordinates x y z from xpt2046
    write_info_touch();

    /*
    float t, a1, a2;

    t = 3300/2; // порог в миливольтах
    fill_buff(0); a1 = buff_ch1[0];
    fill_buff(0); a2 = buff_ch1[0];

    while (!((a1 < t) && (a2 >= t))) // т.е. было ниже порога, а стало больше или равно - был переход через порог
    {
      a1 = a2;
      fill_adc_buff(0); a2 = buff_ch1[0];
    }
    */

    for(uint16_t i = 0; i < BSP_LCD_GetXSize()-1; i++) // measurement
    {
      fill_adc_buff(i);
    }

    for(uint16_t i = 0; i < BSP_LCD_GetXSize()-1; i++) // measurement
    {
      fill_buff(i);
    }

    for(uint16_t i = 0; i < BSP_LCD_GetXSize()-1; i++) // display redraw
    {
      write_graph(i);
    }

    write_grid(); // upgrade grid
    // write_text(i); // upgrade label & voltage measurement
}

void fill_adc_buff(uint16_t i)
{
  if(ts_divide==20); //
  if(ts_divide==50)  delay_nop(7); // 50 us 17
  if(ts_divide==100) delay_nop(41); // 100 us 51
  if(ts_divide==200) delay_nop(108); // 200 us
  if(ts_divide==500) delay_nop(308); // 500 us
  if(ts_divide==1000) delay_nop(641); // 1 ms
  if(ts_divide==2000) delay_nop(1308); // 2 ms
  if(ts_divide==5000) delay_nop(3308); // 5 ms
  if(ts_divide==10000) delay_nop(6641); // 10 ms
  if(ts_divide==20000) delay_nop(13305); // 20 ms
  if(ts_divide==50000) delay_nop(33314); // 50 ms
  if(ts_divide==100000) delay_nop(66683); // 100 ms

  buff_ch1[i] = ADC_Data[ADC_CH*i+0];
  buff_ch2[i] = ADC_Data[ADC_CH*i+1];
  buff_ch3[i] = ADC_Data[ADC_CH*i+2];
  buff_ch4[i] = ADC_Data[ADC_CH*i+3];

  static volatile uint32_t us;
  if(i==4) us = get_time_us();
  if(i==BSP_LCD_GetXSize()-4)
  {
      us = get_time_us() - us;
      // write_info_top(us);
      write_info_down(us);
  }
}

/*
 * save measure adc and fill buffers buff_ray_ch1[i] & real_result_ch1[i]
 * buff_ray_ch1[] use for draw ray with ofset vertical
 * real_result_ch1[] save measure from adc with coefitient for move volts to bars
 */
void fill_buff(uint16_t i)
{
  // if(real_result_ch1[i]<500) real_result_ch1[i] = 0;
  // if(real_result_ch1[i]>500) real_result_ch1[i] -= 500;
  // real_result_ch1[i] *= .4; // значение в барах

  buff_ray_ch1[i] = BSP_LCD_GetXSize()-45-buff_ch1[i]/18; // compute value
  buff_ray_ch2[i] = BSP_LCD_GetXSize()-45-buff_ch2[i]/18;
  buff_ray_ch3[i] = BSP_LCD_GetXSize()-45-buff_ch3[i]/18;
  buff_ray_ch4[i] = BSP_LCD_GetXSize()-45-buff_ch4[i]/18;

  real_result_ch1[i] = buff_ch1[i]*0.82; // 3300/4096=0.8058 compute value
  real_result_ch2[i] = buff_ch2[i]*0.82;
  real_result_ch3[i] = buff_ch3[i]*0.82;
  real_result_ch4[i] = buff_ch4[i]*0.82;

  if(real_result_ch1[i]>max_result_ch1) max_result_ch1 = real_result_ch1[i]; // save max value
  if(real_result_ch2[i]>max_result_ch2) max_result_ch2 = real_result_ch2[i];
  if(real_result_ch3[i]>max_result_ch3) max_result_ch3 = real_result_ch3[i];
  if(real_result_ch4[i]>max_result_ch4) max_result_ch4 = real_result_ch4[i];
}

void rotate_buff(uint16_t r)
{
    volatile uint16_t tmp_ch1, tmp_ch2, tmp_ch3, tmp_ch4;

    for(i = 0; i < r; i++) // scroll buffer ch1 ch2
    {
      tmp_ch1 = buff_ray_ch1[BSP_LCD_GetXSize()-1];
      tmp_ch2 = buff_ray_ch2[BSP_LCD_GetXSize()-1];
      tmp_ch3 = buff_ray_ch3[BSP_LCD_GetXSize()-1];
      tmp_ch4 = buff_ray_ch4[BSP_LCD_GetXSize()-1];

      for(uint16_t i = BSP_LCD_GetXSize()-1; i; i--)
      {
        buff_ray_ch1[i] = buff_ray_ch1[i-1];
        buff_ray_ch2[i] = buff_ray_ch2[i-1];
        buff_ray_ch3[i] = buff_ray_ch3[i-1];
        buff_ray_ch4[i] = buff_ray_ch4[i-1];
      }
      buff_ray_ch1[0] = tmp_ch1;
      buff_ray_ch2[0] = tmp_ch2;
      buff_ray_ch3[0] = tmp_ch3;
      buff_ray_ch4[0] = tmp_ch4;
    }
}

/*
 * draw old ray & draw new ray
 */
void write_graph(uint16_t i)
{
  // write_text(i);
  uint8_t thick_ray = 1; // Thickness ray in pixels

  if(buff_ray_ch1[i]<32) buff_ray_ch1[i]=32; // restriction on Y max
  if(buff_ray_ch2[i]<32) buff_ray_ch2[i]=32;
  if(buff_ray_ch3[i]<32) buff_ray_ch3[i]=32;
  if(buff_ray_ch4[i]<32) buff_ray_ch4[i]=32;

  if(buff_ray_ch1[i]>BSP_LCD_GetYSize()-32) buff_ray_ch1[i]=BSP_LCD_GetYSize()-32; // restriction on Y min
  if(buff_ray_ch2[i]>BSP_LCD_GetYSize()-32) buff_ray_ch2[i]=BSP_LCD_GetYSize()-32;
  if(buff_ray_ch3[i]>BSP_LCD_GetYSize()-32) buff_ray_ch3[i]=BSP_LCD_GetYSize()-32;
  if(buff_ray_ch4[i]>BSP_LCD_GetYSize()-32) buff_ray_ch4[i]=BSP_LCD_GetYSize()-32;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  if(i>0 && i < BSP_LCD_GetXSize()-0) // clean old ray
  {
    tft_draw_dot_line(i+1, buff_clean_ch1[i+1], i, buff_clean_ch1[i], thick_ray); // draw ray old
    tft_draw_dot_line(i+1, buff_clean_ch2[i+1], i, buff_clean_ch2[i], thick_ray);
    tft_draw_dot_line(i+1, buff_clean_ch3[i+1], i, buff_clean_ch3[i], thick_ray);
    tft_draw_dot_line(i+1, buff_clean_ch4[i+1], i, buff_clean_ch4[i], thick_ray);
  }

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  if(i>2 && i < BSP_LCD_GetXSize()-2) // draw new ray
  {
    tft_draw_dot_line(i, buff_ray_ch1[i], i-1, buff_ray_ch1[i-1], thick_ray); // draw ray new
  }

  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  if(i>2 && i < BSP_LCD_GetXSize()-2) // draw new ray
  {
    tft_draw_dot_line(i, buff_ray_ch2[i], i-1, buff_ray_ch2[i-1], thick_ray); // draw ray new
  }

  BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA);
  if(i>2 && i < BSP_LCD_GetXSize()-2) // draw new ray
  {
    tft_draw_dot_line(i, buff_ray_ch3[i], i-1, buff_ray_ch3[i-1], thick_ray); // draw ray new
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  if(i>2 && i < BSP_LCD_GetXSize()-2) // draw new ray
  {
    tft_draw_dot_line(i, buff_ray_ch4[i], i-1, buff_ray_ch4[i-1], thick_ray); // draw ray new
  }

  /*
   * The beam buffer is stored in the cleaning buffer
   */
  buff_clean_ch1[i]=buff_ray_ch1[i];
  buff_clean_ch2[i]=buff_ray_ch2[i];
  buff_clean_ch3[i]=buff_ray_ch3[i];
  buff_clean_ch4[i]=buff_ray_ch4[i];

  static volatile uint32_t ms;
  if(i==4) ms = get_time_ms();
  if(i==BSP_LCD_GetXSize()-4)
  {
      ms = get_time_ms() - ms;
      write_info_top(ms);
  }
}

/*
 * drawing of a coordinate grid
 */
void write_grid(void)
{
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  for(uint16_t x = 32;x<BSP_LCD_GetYSize()-32;x+=4)
      for(uint16_t i = 4; i<BSP_LCD_GetXSize()-4; i+=31)
      {
        BSP_LCD_FillRect(i, x, 1,1); // draw vertical dot line
      }

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  for(uint16_t x = 56;x<BSP_LCD_GetYSize()-56;x+=31)for(uint16_t i = 1; i<BSP_LCD_GetXSize()-1; i+=4)BSP_LCD_FillRect(i, x, 1,1); // draw horizontal dot line
}

void tft_draw_dot_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t solid)
{
  uint8_t DOT_SIZE = 3; // space between draw points

  int xerr=0,yerr=0,delta_y,delta_x,distance;
  int incx,incy,uCol,uRow;

  delta_x=x2-x1;
  delta_y=y2-y1;
  uRow=x1;
  uCol=y1;

  if(delta_y>0)
  {
    incx=DOT_SIZE;
  }
  else if(delta_y==0)
  {
    incx=0;
  }
  else
  {
    incx=-DOT_SIZE;
    delta_y=-delta_y;
  }

  if(delta_x>0)
  {
    incy=DOT_SIZE;
  }
  else if(delta_x==0)
  {
    incy=0;
  }
  else
  {
    incy=-DOT_SIZE;
    delta_x=-delta_x;
  }

  if( delta_y>delta_x)
  {
      distance=delta_y;
  }
  else
  {
    distance=delta_x;
  }

  for(uint16_t t=0;t<=distance+1;t+=DOT_SIZE )
  {
    BSP_LCD_FillRect(uRow,uCol, solid,solid);
    xerr+=delta_y ;
    yerr+=delta_x ;

    if(xerr>distance)
    {
      xerr-=distance;
      uCol+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uRow+=incy;
    }
  }
}

