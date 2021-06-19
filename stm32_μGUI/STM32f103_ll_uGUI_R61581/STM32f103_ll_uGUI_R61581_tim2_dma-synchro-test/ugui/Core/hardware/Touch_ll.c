/*
 * Touch_ll.c
 *
 *  Created on: 28.10.2018
 *      Author: Harebit
 */

#include "Touch.h"
#include "ugui.h"
// #include "stm32f1xx_hal_adc.h"
// #include "adc_cfg.h"
#include "stdlib.h"

#ifdef __cplusplus
 extern "C" {
#endif

/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // activate for timer interrupt -> don't work well
{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			GUI_TOUCH_Exec();
			GUI_Exec();
}*/

void Init_ADC_Pins(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_ANALOG);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC2);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(XM_GPIO_Port, XM_Pin, LL_GPIO_MODE_ANALOG);
}

void DeInit_YP_Pin(void)
{
   LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_FLOATING);
   LL_GPIO_SetPinPull(YP_GPIO_Port, YP_Pin, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(YP_GPIO_Port, YP_Pin, LL_GPIO_SPEED_FREQ_HIGH);
   LL_GPIO_SetPinOutputType(YP_GPIO_Port, YP_Pin, LL_GPIO_OUTPUT_PUSHPULL);
}

int16_t ugui_TOUCH_MeasureZ(void) // for X measurements
{
   GPIO_Z_MeasurementSetup();
   pressure = GPIO_Z_Measurement();
   return pressure;
}

uint16_t ugui_TOUCH_MeasureX(void)
{
   int xValue = 0;
   GPIO_X_MeasurementSetup();
   xValue = GPIO_X_Measurement();
   // printf("xValue: %i\t", xValue);
   return xValue;
}

uint16_t ugui_TOUCH_MeasureY(void)
{
   int yValue = 0;
   GPIO_Y_MeasurementSetup();
   yValue = GPIO_Y_Measurement();
   // printf("yValue: %i\t", yValue);
   return yValue;
}

uint8_t ugui_TOUCH_StateZ(void)
{
   pressure = ugui_TOUCH_MeasureZ();
   if(pressure > MIN_PRESSURE)
      return TOUCH_STATE_PRESSED;
   else return TOUCH_STATE_RELEASED;
}

int16_t ugui_TOUCH_Py(void)
{
   int16_t Py;
   Py = ((ugui_TOUCH_MeasureX())- TOUCH_AD_RIGHT)* (XSIZE_PHYS-1)/(TOUCH_AD_LEFT-TOUCH_AD_RIGHT);
   if (Py < 0) Py = 0;
   if (Py > XSIZE_PHYS) Py = XSIZE_PHYS;
   return Py;
}

/* ***** attention! ***************************************************
 * x and y turned ∠АOC = 90° to right
*************** on Display ->  x = DISPLAY_WIDTH 240
*			  *			   |   y = DISPLAY_HEIGHT 320
*			  *            v
*			  *
*			  *
*			  *
*			  *
*			  *
*			  * 			^
*			  * Measurement |   x = XSIZE_PHYS 320
***************				->  y = YSIZE_PHYS 240
*/

int16_t ugui_TOUCH_Px(void)
{
   int16_t Px;
   Px = ((ugui_TOUCH_MeasureY()- TOUCH_AD_BOTTOM) * (YSIZE_PHYS-1)/(TOUCH_AD_TOP-TOUCH_AD_BOTTOM));
   if (Px < 0) Px = 0;
   if (Px > YSIZE_PHYS) Px = YSIZE_PHYS;
   return Px;
}

void Touch_ADC_Init(void)
{
    ADC_Init(ADC1);
    GPIO_Restore_Outputs();
    // HAL_TIM_Base_Start_IT(&htim3);
}

// GPIO_PinState P_PinState;
// GPIO_PinState M_PinState;

void GPIO_X_MeasurementSetup(void)
{
    // Y axis YP input  YM disconnect
    // P_PinState = ((XP_GPIO_Port->ODR & XP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    // M_PinState = ((XM_GPIO_Port->ODR & XM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(YM_GPIO_Port, YM_Pin, LL_GPIO_MODE_INPUT);

    // X axis output
    LL_GPIO_SetPinMode(XM_GPIO_Port, XM_Pin, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetOutputPin(XP_GPIO_Port, XP_Pin);
    LL_GPIO_ResetOutputPin(XM_GPIO_Port, XM_Pin);
}

uint16_t GPIO_X_Measurement(void)
{
    int16_t g_ADCValue;
    uint16_t sValues[NUMSAMPLES];
    uint16_t xValue;

    for (int i = 0; i < NUMSAMPLES; i++)
    {
      g_ADCValue = adc_get_value(ADC1, 8);
      sValues[i] = g_ADCValue;
    }

    // HAL_GPIO_WritePin(YP_GPIO_Port, XP_Pin, P_PinState);
    // HAL_GPIO_WritePin(YM_GPIO_Port, XM_Pin, M_PinState);
    GPIO_Restore_Outputs();
    xValue =  4096-((sValues[0]+sValues[1]+sValues[2]+sValues[3]+sValues[4])/NUMSAMPLES);

    return xValue;
}

void GPIO_Y_MeasurementSetup(void)
{
    // *** X axis XM input  XP disconnect
    // P_PinState = ((YP_GPIO_Port->ODR & YP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    // M_PinState = ((YM_GPIO_Port->ODR & YM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LL_GPIO_SetPinMode(XM_GPIO_Port, XM_Pin, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(XP_GPIO_Port, XP_Pin, LL_GPIO_MODE_INPUT);

    // *** Y axis output
    LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(YM_GPIO_Port, YM_Pin, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetOutputPin(YP_GPIO_Port, YP_Pin);
    LL_GPIO_ResetOutputPin(YM_GPIO_Port, YM_Pin);
}

uint16_t GPIO_Y_Measurement(void)
{
    int16_t g_ADCValue;
    uint16_t sValues[NUMSAMPLES];
    uint16_t yValue;

    for (int i = 0; i < NUMSAMPLES; i++)
    {
      g_ADCValue = adc_get_value(ADC1, 9);
      sValues[i] = g_ADCValue;
    }

    // HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, P_PinState);
    // HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, M_PinState);

    GPIO_Restore_Outputs();
    yValue = ((sValues[0]+sValues[1]+sValues[2]+sValues[3]+sValues[4])/NUMSAMPLES);

    return yValue;
}

void GPIO_Z_MeasurementSetup(void)
{
    // XM input YP input
    // P_PinState = ((XP_GPIO_Port->ODR & XP_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    // M_PinState = ((YM_GPIO_Port->ODR & YM_Pin) != 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LL_GPIO_SetPinMode(XM_GPIO_Port, XM_Pin, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_ANALOG);

    // XP output YM output
    LL_GPIO_SetPinMode(XP_GPIO_Port, XP_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(YM_GPIO_Port, YM_Pin, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_ResetOutputPin(XP_GPIO_Port, XP_Pin);
    LL_GPIO_SetOutputPin(YM_GPIO_Port, YM_Pin);
}

int16_t GPIO_Z_Measurement(void)
{
    uint8_t i;
    int16_t g_ADCValue1;
    int16_t g_ADCValue2;
    uint16_t sValues1[NUMSAMPLES];
    uint16_t sValues2[NUMSAMPLES];
    int16_t pressure;

    for (i = 0; i < NUMSAMPLES; i++)
    {
      g_ADCValue1 = adc_get_value(ADC1, 8);
      g_ADCValue2 = adc_get_value(ADC1, 9);
      sValues1[i] = g_ADCValue1;
      sValues2[i] = g_ADCValue2;
    }

    uint16_t z1 = (sValues1[0]+sValues1[1]+sValues1[2]+sValues1[3]+sValues1[4])/NUMSAMPLES;
    uint16_t z2 = (sValues2[0]+sValues2[1]+sValues2[2]+sValues2[3]+sValues2[4])/NUMSAMPLES;

    /* printf("z1: %i\t", z1);
       printf("z1: %i\t", z2);*/

    pressure = abs((4096+z2-z1)/2);
    // printf("pressure: %i\n", pressure);

    // HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, P_PinState);
    // HAL_GPIO_WritePin(YM_GPIO_Port, YM_Pin, M_PinState);

    GPIO_Restore_Outputs();
    return pressure;
}

void GPIO_Restore_Outputs(void)
{
    LL_GPIO_SetPinMode(XM_GPIO_Port, XM_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(YP_GPIO_Port, YP_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(XP_GPIO_Port, XP_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(YM_GPIO_Port, YM_Pin, LL_GPIO_MODE_OUTPUT);
}

#ifdef __cplusplus
}
#endif // __cplusplus
