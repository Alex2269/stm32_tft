#ifndef __generator_H
#define __generator_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -------*/
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"

/* USER CODE BEGIN Includes */

uint16_t timx_counter = 0;
uint16_t tim3_counter = 0;
uint16_t tim4_counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM3)
  {
    tim3_counter++;
    timx_counter++;
    if(timx_counter>30) // 
    {
      timx_counter=0;
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }

    if(tim3_counter>60)
    {
      timx_counter=0; // increment stoped
    }

    if(tim3_counter>300)
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
      //HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }
    if(tim4_counter>40)
    {
      tim4_counter=0;
      //HAL_GPIO_WritePin(DSP_LED_GPIO_Port, DSP_LED_Pin, GPIO_PIN_SET);
    }
  }
  //--
}

#ifdef __cplusplus
}
#endif

#endif /* __generator_H */
