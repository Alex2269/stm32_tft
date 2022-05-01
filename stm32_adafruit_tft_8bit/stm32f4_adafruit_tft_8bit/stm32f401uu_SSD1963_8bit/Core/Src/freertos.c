/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SSD1963.h"
#include "c_images_all_includes.h"

#include "stm32_tick.h"
#include "time.h"
#include "semphr.h"
#include "lissajous.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_01 */
osThreadId_t Task_01Handle;
const osThreadAttr_t Task_01_attributes = {
  .name = "Task_01",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_02 */
osThreadId_t Task_02Handle;
const osThreadAttr_t Task_02_attributes = {
  .name = "Task_02",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Task_03 */
osThreadId_t Task_03Handle;
const osThreadAttr_t Task_03_attributes = {
  .name = "Task_03",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_04 */
osThreadId_t Task_04Handle;
const osThreadAttr_t Task_04_attributes = {
  .name = "Task_04",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Display_Mutex */
osMutexId_t Display_MutexHandle;
const osMutexAttr_t Display_Mutex_attributes = {
  .name = "Display_Mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void draw_window(void)
{
  osMutexWait(Display_MutexHandle, osWaitForever); // mutex on

  // draw petals/leaf`s left top - for fun
  for(float a = 0; a <6.28318530718; a += 0.01)
  {
   for(float r1 = 2; r1 <18; r1 ++)
   {
    float r = r1 * (1 + sin(a)) * (1 + 0.9 * cos(8 * a)) * (1 + 0.1 * cos(24 * a));
    fillRect(180-(uint16_t)(100 + r * cos(a)), 190 - (uint16_t)(100 + r * sin(a)),3,3, GREEN);
   }
  }
  for(float a = 0; a <6.28318530718; a += 0.01)
  {
    float r = 18 * (1 + sin(a)) * (1 + 0.9 * cos(8 * a)) * (1 + 0.1 * cos(24 * a));
    fillRect(180-(uint16_t)(100 + r * cos(a)), 190 - (uint16_t)(100 + r * sin(a)),3,3, OLIVE);
  }

  fillRoundRect(8, Display_Get_HEIGHT()-42, Display_Get_WIDTH()-15, 36, 6, DARKBLUE);

  setTextSize(1);
  setCursor(60, Display_Get_HEIGHT()-40);
  setTextColor(CYAN);
  setTextBgColor(DARKBLUE);
  UG_FONT_set(&font_ClearSans_36X38);
  UG_PutString("STATUS BAR TEST");
  UG_FONT_set(&font_BebasNeue_41X59);

  drawRoundRect(6, Display_Get_HEIGHT()-43, Display_Get_WIDTH()-11, 38, 6, RED);

  drawRoundRect(1, 5, Display_Get_WIDTH()-2, Display_Get_HEIGHT()-6,  10, BLUE);
  drawRoundRect(2, 6, Display_Get_WIDTH()-4, Display_Get_HEIGHT()-8,  10, YELLOW);
  drawRoundRect(3, 7, Display_Get_WIDTH()-6, Display_Get_HEIGHT()-10, 10, CYAN);
  drawRoundRect(4, 8, Display_Get_WIDTH()-8, Display_Get_HEIGHT()-12, 10, MAGENTA);

  // drawRoundRect(97,  9, 76, 92, 8, RED);
  // drawRoundRect(98, 10, 74, 90, 8, GREEN);
  // drawRoundRect(99, 11, 72, 88, 8, CYAN);

  #define left 302
  #define win_width 162
  fillRoundRect(left+3, 11, win_width+2, 88, 8, DARKSLATEGRAY);
  drawRoundRect(left+1,  9, win_width+6, 92, 8, RED);
  drawRoundRect(left+2, 10, win_width+4, 90, 8, GREEN);
  drawRoundRect(left+3, 11, win_width+2, 88, 8, CYAN);

  #define y_set 112
  drawRoundRect(5, y_set+1, Display_Get_WIDTH()-10,  y_set+8, 8, RED);
  drawRoundRect(6, y_set+2, Display_Get_WIDTH()-12,  y_set+6, 8, GREEN);
  drawRoundRect(7, y_set+3, Display_Get_WIDTH()-14,  y_set+4, 8, CYAN);

  osMutexRelease(Display_MutexHandle); // mutex off
}

void draw_dot_grid(void)
{
  // osMutexAcquire(Display_MutexHandle, portMAX_DELAY);
  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  for(uint16_t y = 106;y<Display_Get_HEIGHT()-270;y+= 8)
    for(uint16_t x = 16; x<Display_Get_WIDTH()-15; x+=32) fillRect(x, y, 2,2, CYAN);
  for(uint16_t y = 106;y<Display_Get_HEIGHT()-270;y+=32)
    for(uint16_t x = 16; x<Display_Get_WIDTH()-15; x+=8 ) fillRect(x, y, 2,2, CYAN);
  osMutexRelease(Display_MutexHandle); // mutex off
}

void draw_dynamic_grid(int16_t shiftx)
{
  if(shiftx < 12) return;
  #define y_pos  124
  #define ceil_x  33
    // osMutexAcquire(Display_MutexHandle, portMAX_DELAY);
  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  for(uint16_t y = y_pos;y<y_pos+97;y+= 8)
    for(uint16_t x = shiftx; x<ceil_x+shiftx; x+=32) fillRect(x, y, 2,2, CYAN);
  for(uint16_t y = y_pos;y<y_pos+97;y+=32)
    for(uint16_t x = shiftx; x<ceil_x+shiftx; x+=8 ) fillRect(x, y, 2,2, CYAN);
  osMutexRelease(Display_MutexHandle); // mutex off
}

// char input_buff[MAX_INPUT_LEN] = {};

void analog_read(void)
{
  static volatile uint16_t Value0;
  static volatile uint16_t Value1;
  static volatile uint16_t Value2;
  static volatile uint16_t Value3;
  static volatile uint16_t Value4;
  static volatile uint16_t Value5;
  static volatile uint16_t Value6;
  static volatile uint16_t Value7;

  Value1 = read_adc(ADC1, GPIOA, 0x1UL << 1);
  Value2 = read_adc(ADC1, GPIOA, 0x1UL << 2);
  Value3 = read_adc(ADC1, GPIOA, 0x1UL << 3);
  Value4 = read_adc(ADC1, GPIOA, 0x1UL << 4);
  Value5 = read_adc(ADC1, GPIOA, 0x1UL << 5);
  Value6 = read_adc(ADC1, GPIOA, 0x1UL << 6);
  Value7 = read_adc(ADC1, GPIOA, 0x1UL << 7);

  static volatile uint16_t x_pos = 16;

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  ///!! fillRect( x_pos + 64, 11, 88, 31, GRAY);
  osMutexRelease(Display_MutexHandle); // mutex off

  setTextColor(YELLOW);
  setTextBgColor(GRAY);

  /**
  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(64,   248);
  setTextSize(1);
  UG_FONT_set(&font_BebasNeue_41X59);
  draw_value(Value1);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100);

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(64,   304);
  setTextSize(1);
  UG_FONT_set(&font_BebasNeue_41X59);
  draw_value(Value2);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100);

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(64,   360);
  setTextSize(1);
  UG_FONT_set(&font_BebasNeue_41X59);
  draw_value(Value3);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100); **/

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(300,   240);
  setTextSize(1);
  UG_FONT_set(&font_ClearSans_22X22);
  draw_fvalue(Value4,0);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100);

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(300,   260);
  setTextSize(1);
  UG_FONT_set(&font_ClearSans_22X22);
  draw_fvalue(Value5,0);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100);

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  setCursor(300,   280);
  setTextSize(1);
  UG_FONT_set(&font_ClearSans_22X22);
  draw_fvalue(Value6,0);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(100);

  /** osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  gauge(644,268, 0, YELLOW, (read_adc(ADC1, GPIOA, 0x1UL << 0)/36)+270 );
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(150); **/
}

void graph_line(uint16_t xpos)
{
  char buff_time[12];
  static volatile uint32_t tick_ms;
  static volatile uint32_t time_ms;
  static volatile uint16_t x_shift = 160;

  if( xpos == 8 )
  {

    tick_ms += ticks_get()/84000; // read chronometry
    if(ticks_get()>8e6) ticks_set(0); // start chronometry
    osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
    setCursor(x_shift, 16);
    setTextColor(GREEN);
    setTextSize(1);
    UG_FONT_set(&font_BebasNeue_15X22);
    draw_value(tick_ms); UG_PutString(" ms");
    UG_FONT_set(&font_BebasNeue_41X59);
    setTextColor(YELLOW);
    osMutexRelease(Display_MutexHandle); // mutex off

    osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
    time_ms = get_time_new_ms(); // read chronometry
    setCursor(x_shift, 60);
    setTextColor(CYAN);
    setTextSize(1);
    UG_FONT_set(&font_BebasNeue_15X22);
    draw_value(time_ms); UG_PutString(" m");
    UG_FONT_set(&font_BebasNeue_41X59);
    setTextColor(YELLOW);
    osMutexRelease(Display_MutexHandle); // mutex off
  }

  #define BUFSIZE 800
  static volatile uint16_t buff_1_draw[BUFSIZE] = {0};
  static volatile uint16_t buff_2_draw[BUFSIZE] = {0};
  static volatile uint16_t buff_3_draw[BUFSIZE] = {0};
  static volatile uint16_t buff_1_clean[BUFSIZE] = {0};
  static volatile uint16_t buff_2_clean[BUFSIZE] = {0};
  static volatile uint16_t buff_3_clean[BUFSIZE] = {0};

  buff_1_draw[xpos] = read_adc(ADC1, GPIOA, 0x1UL << 4)/36+126;
  buff_2_draw[xpos] = read_adc(ADC1, GPIOA, 0x1UL << 5)/36+128;
  buff_3_draw[xpos] = read_adc(ADC1, GPIOA, 0x1UL << 6)/36+130;

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_color(xpos, buff_1_draw, buff_1_clean, 3, CYAN,GRAY);
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_color(xpos, buff_2_draw, buff_2_clean, 3, YELLOW,GRAY);
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_color(xpos, buff_3_draw, buff_3_clean, 3, GREEN,GRAY);
  osMutexRelease(Display_MutexHandle); // mutex off
  osDelay(5);
}

void draw_row_level(void)
{
  static volatile uint16_t y_offset, x_offset, space_width,level_width;
  static volatile uint16_t val0,val1,val2,val3,val4,val5,val6,val7;
  static volatile uint16_t clear0,clear1,clear2,clear3,clear4,clear5,clear6,clear7;

  x_offset = 324;
  y_offset = 84;
  space_width = 16;
  level_width = 10;

  // ADC_Init(ADC1);
  // osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  // osMutexWait(Display_MutexHandle, osWaitForever); // mutex on

  val0 = read_adc(ADC1, GPIOA, 0x1UL << 0)/40;
  val1 = read_adc(ADC1, GPIOA, 0x1UL << 1)/40;
  val2 = read_adc(ADC1, GPIOA, 0x1UL << 2)/40;
  val3 = read_adc(ADC1, GPIOA, 0x1UL << 3)/40;
  val4 = read_adc(ADC1, GPIOA, 0x1UL << 4)/40;
  val5 = read_adc(ADC1, GPIOA, 0x1UL << 5)/40;
  val6 = read_adc(ADC1, GPIOA, 0x1UL << 6)/40;
  val7 = read_adc(ADC1, GPIOA, 0x1UL << 7)/40;

  // clear measuring columns:
  // read measuring PA0-PA8 ADC input

  // draw measuring columns:
  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 0, y_offset, x_offset + space_width * 0, y_offset - clear0, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 0, y_offset, x_offset + space_width * 0, y_offset - val0, level_width, OLIVE);
  clear0 = val0;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 1, y_offset, x_offset + space_width * 1, y_offset - clear1, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 1, y_offset, x_offset + space_width * 1, y_offset - val1, level_width, SILVER);
  clear1 = val1;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 2, y_offset, x_offset + space_width * 2, y_offset - clear2, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 2, y_offset, x_offset + space_width * 2, y_offset - val2, level_width, TEAL);
  clear2 = val2;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 3, y_offset, x_offset + space_width * 3, y_offset - clear3, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 3, y_offset, x_offset + space_width * 3, y_offset - val3, level_width, GREEN);
  clear3 = val3;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 4, y_offset, x_offset + space_width * 4, y_offset - clear4, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 4, y_offset, x_offset + space_width * 4, y_offset - val4, level_width, CYAN);
  clear4 = val4;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 5, y_offset, x_offset + space_width * 5, y_offset - clear5, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 5, y_offset, x_offset + space_width * 5, y_offset - val5, level_width, MAGENTA);
  clear5 = val5;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 6, y_offset, x_offset + space_width * 6, y_offset - clear6, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 6, y_offset, x_offset + space_width * 6, y_offset - val6, level_width, ORANGE);
  clear6 = val6;
  osMutexRelease(Display_MutexHandle); // mutex off

  osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
  write_graph_dot_line(x_offset + space_width * 7, y_offset, x_offset + space_width * 7, y_offset - clear7, level_width, GRAY);
  write_graph_dot_line(x_offset + space_width * 7, y_offset, x_offset + space_width * 7, y_offset - val7, level_width, GREEN);
  clear7 = val7;
  osMutexRelease(Display_MutexHandle); // mutex off

  osDelay(200);
}

/* USER CODE END FunctionPrototypes */

void StartTask_01(void *argument);
void StartTask_02(void *argument);
void StartTask_03(void *argument);
void StartTask_04(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of Display_Mutex */
  Display_MutexHandle = osMutexNew(&Display_Mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_01 */
  Task_01Handle = osThreadNew(StartTask_01, NULL, &Task_01_attributes);

  /* creation of Task_02 */
  Task_02Handle = osThreadNew(StartTask_02, NULL, &Task_02_attributes);

  /* creation of Task_03 */
  Task_03Handle = osThreadNew(StartTask_03, NULL, &Task_03_attributes);

  /* creation of Task_04 */
  Task_04Handle = osThreadNew(StartTask_04, NULL, &Task_04_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask_01 */
/**
  * @brief  Function implementing the Task_01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_01 */
void StartTask_01(void *argument)
{
  /* USER CODE BEGIN StartTask_01 */
  draw_window();
  ticks_init();
  ticks_set(0); // start chronometry
  save_time_ms(); // start chronometry
  #include "angle.h"
  Point point;
  point.x1 = 350;
  point.y1 = 420;
  point.ro = 175;
  uint16_t old_angle;

  draw_arch(point.x1, point.y1, 0, 180, 50, 2, OLIVE);
  draw_arch(point.x1, point.y1, 0, 180, 80, 2, OLIVE);

  /* Infinite loop */
  for(;;)
  {
    for(uint16_t i =0;i<=180; i++)
    {
      osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on

      point.angle = old_angle;
      draw_arrow(&point, 96, 2, GRAY); // directional arrow clar
      draw_arch(point.x1, point.y1, point.angle -5, point.angle +5, 50, 2, OLIVE);
      draw_arch(point.x1, point.y1, point.angle -3, point.angle +3, 80, 2, OLIVE);

      point.angle = i;
      draw_arrow(&point, 96, 2, ORANGE); // directional arrow draw
      draw_arch(point.x1, point.y1, point.angle -5, point.angle +5, 50, 2, OLIVE);
      draw_arch(point.x1, point.y1, point.angle -3, point.angle +3, 80, 2, OLIVE);

      old_angle = point.angle;

      osMutexRelease(Display_MutexHandle); // mutex off

      for(uint16_t i=0;i<=180;i+=30)
      {
        osMutexWait(Display_MutexHandle, portMAX_DELAY); // mutex on
        point.angle = i;
        get_angle_pos(&point);
        // write_graph_dot_line(point.x1,point.y1, point.x2,point.y2, 1, GREEN);
        for(uint16_t i=100;i<=110;i++) draw_angle(&point, i, 2, RED); // yellow tags
        for(uint16_t i=110;i<=116;i++) draw_angle(&point, i, 2, GREEN); // yellow tags
        osMutexRelease(Display_MutexHandle); // mutex off
        osDelay(1);
      }

      osDelay(50);
    }

    osDelay(50);
  }

  /* USER CODE END StartTask_01 */
}

/* USER CODE BEGIN Header_StartTask_02 */
/**
* @brief Function implementing the Task_02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_02 */
void StartTask_02(void *argument)
{
  /* USER CODE BEGIN StartTask_02 */
  static volatile uint16_t xpos;

  for(int16_t i=8;i<Display_Get_WIDTH()-8;i++)
  {
    if( !(i % 32) ) draw_dynamic_grid(i-33);
  }

  // char buff_time[12];
  // static volatile uint32_t tick_ms;

  /* Infinite loop */
  for(;;)
  {
    xpos++;
    if(xpos >= Display_Get_WIDTH()-8) // start chronometry
    {
      xpos = 8;
      osDelay(10);
      ticks_set(0);
    }
    if( !(xpos % 32) ) draw_dynamic_grid(xpos-33);
    // --
    // if( !(xpos % 400) ) analog_read();
    // if( xpos == 8 ) analog_read();
    // --
    // if( !(xpos % 400) )

    graph_line(xpos);

    osDelay(5);
    // --
  }
  /* USER CODE END StartTask_02 */
}

/* USER CODE BEGIN Header_StartTask_03 */
/**
* @brief Function implementing the Task_03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_03 */
void StartTask_03(void *argument)
{
  /* USER CODE BEGIN StartTask_03 */
  /* Infinite loop */
  for(;;)
  {
    // if( !(count % 400) ) analog_read();
    // if( count == 400 ) analog_read();
    analog_read();

    osDelay(25);
  }
  /* USER CODE END StartTask_03 */
}

/* USER CODE BEGIN Header_StartTask_04 */
/**
* @brief Function implementing the Task_04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_04 */
void StartTask_04(void *argument)
{
  /* USER CODE BEGIN StartTask_04 */

  /* Infinite loop */
  for(;;)
  {
    // draw_row_level();
    // lissajous(385,52, 0.125, 0.250, CYAN , DARKSLATEGRAY);

    // drawRGBBitmap(160, 140, RGBY);
    // drawRGBBitmap(400, 260, lion);
    // drawRGBBitmap(10, 235, tiger);

    // setTextColor(BLUE);
    // setTextBgColor(GREEN);
    // setCursor(300, 300);
    // draw_fvalue(15.0,2);
    // drawChar(300, 300, 'A', RED, 0, 6);
    // writeString("Hello");

    // osDelay(100);
  }
  /* USER CODE END StartTask_04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
