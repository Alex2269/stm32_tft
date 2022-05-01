#ifndef __timers_calback_H
#define __timers_calback_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -------*/
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "osc.h"
#include "adc.h"
#include "tim.h"

#define show_fps 0

/* USER CODE BEGIN Includes */

uint16_t timx_counter = 0;
uint16_t tim3_counter = 0;
uint16_t tim4_counter = 0;
extern uint16_t fps_count;
extern uint16_t buff_scroll;
uint16_t clean = 0;

extern uint16_t tft_W;
extern uint16_t tft_H;

char label_fps[4] = "fps";

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM3)
  {
    tim3_counter++;
    timx_counter++;
    if(timx_counter>100) // 
    {
      timx_counter=0;
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }

    if(tim3_counter>200)
    {
      timx_counter=0; // increment stoped
    }

    if(tim3_counter>1000)
    {
      tim3_counter=0;
      HAL_GPIO_WritePin(DSP_LED_GPIO_Port, DSP_LED_Pin, GPIO_PIN_SET);
    }
  }
  //--
  if(tim_baseHandle->Instance==TIM4)
  {
    tim4_counter++;
    if(tim4_counter<4)
    {
      // HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }
    if(tim4_counter>1000)
    {
      tim4_counter=0;
      clean++;
      if(clean>15)
      {
        clean=0;
        // write_statusbar();
        // tft_fill_grid(24/*y1*/, 0/*x1*/, 240-24/*y2*/, tft_W/*x2*/, COLOR_BLACK);
      }
      #if show_fps
        write_string((tft_W-100), tft_H-24, (int8_t*)label_fps);
        write_value((tft_W-100)+60, tft_H-24, fps_count);
      #endif
      fps_count=0;
      // HAL_GPIO_WritePin(DSP_LED_GPIO_Port, DSP_LED_Pin, GPIO_PIN_SET);
    }
  }
  //--
}

#ifdef __cplusplus
}
#endif

#endif /* __timers_calback_H */
