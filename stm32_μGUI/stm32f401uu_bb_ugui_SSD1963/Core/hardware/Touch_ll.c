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

#define MODE_INPUT                 (0U) /*!< Select input mode */
#define MODE_OUTPUT                0x1UL  /*!< Select output mode */
#define MODE_ALTERNATE             0x2UL  /*!< Select alternate function mode */
#define MODE_ANALOG                0x3UL    /*!< Select analog mode */

static inline
void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  MODIFY_REG(GPIOx->MODER, (3U << (POSITION_VAL(1<<Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
}

static inline
void SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, PinMask);
}

static inline
void ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, (PinMask << 16));
}

void Init_ADC_Pins(void) // this func no use
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_APB2_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_ANALOG);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_APB2_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_ANALOG);
}

uint16_t ugui_TOUCH_MeasureX(void)
{
   int Value = 0;
   GPIO_X_MeasurementSetup();
   Value = GPIO_Measurement(ADC1, XM_Pin);
   // printf("Value: %i\t", Value);
   return Value;
}

uint16_t ugui_TOUCH_MeasureY(void)
{
   int Value = 0;
   GPIO_Y_MeasurementSetup();
   Value = GPIO_Measurement(ADC1, YP_Pin);
   // printf("Value: %i\t", Value);
   return Value;
}

int16_t ugui_TOUCH_MeasureZ(void) // for X measurements
{
   int16_t z1 = 0;
   int16_t z2 = 0;
   int16_t pressure;
   GPIO_Z_MeasurementSetup();
   z1 = GPIO_Measurement(ADC1, XM_Pin);
   z2 = GPIO_Measurement(ADC1, YP_Pin);
   pressure = abs((4096+z2-z1)/2);
   // printf("pressure: %i\n", pressure);
   return pressure;
}

uint8_t ugui_TOUCH_StateZ(void)
{
   pressure = ugui_TOUCH_MeasureZ();
   if(pressure > MIN_PRESSURE)
      return TOUCH_STATE_PRESSED;
   else return TOUCH_STATE_RELEASED;
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
*			  * Measurement |   x = DISPLAY_WIDTH 320
***************				->  y = DISPLAY_HEIGHT 240
*/

int16_t ugui_TOUCH_Px(void)
{
   int16_t Px;
   Px = ((ugui_TOUCH_MeasureY()- TOUCH_AD_BOTTOM) * (DISPLAY_HEIGHT-1)/(TOUCH_AD_TOP-TOUCH_AD_BOTTOM));
   if (Px < 0) Px = 0;
   if (Px > DISPLAY_HEIGHT) Px = DISPLAY_HEIGHT;
   return Px;
}

void GPIO_X_MeasurementSetup(void)
{
    // write_pin(PortB,CS,1);
    SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);

    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_INPUT);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_ANALOG);

    // X axis output
    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_OUTPUT);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);

    SetOutputPin(XP_GPIO_Port, XP_Pin);
    ResetOutputPin(XM_GPIO_Port, XM_Pin);
}

void GPIO_Y_MeasurementSetup(void)
{
    // write_pin(PortB,CS,1);
    SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);

    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_ANALOG);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_INPUT);

    // *** Y axis output
    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_OUTPUT);

    SetOutputPin(YP_GPIO_Port, YP_Pin);
    ResetOutputPin(YM_GPIO_Port, YM_Pin);
}

void GPIO_Z_MeasurementSetup(void)
{
    // write_pin(PortB,CS,1);
    SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);

    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_ANALOG);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_ANALOG);

    // XP output YM output
    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);

    ResetOutputPin(XP_GPIO_Port, XP_Pin);
    SetOutputPin(YM_GPIO_Port, YM_Pin);
}

uint16_t GPIO_Measurement(ADC_TypeDef *ADCx, uint8_t channel)
{
    uint16_t Value = 0;
    for (int i = 0; i < 16; i++) // (65535/4095) == 16 max for uint16_t
    {
      Value += adc_get_value(ADC1, YP_Pin);
    }
    GPIO_Restore_Outputs();
    return Value/16;
}

void GPIO_Restore_Outputs(void)
{
    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_OUTPUT);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);
    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_OUTPUT);
}

#ifdef __cplusplus
}
#endif // __cplusplus
