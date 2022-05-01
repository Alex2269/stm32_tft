#include "stm32f4xx.h"
#include "Touch.h"
#include "ugui.h"
#include "stdbool.h"
#include "stdlib.h"
// #include "adc_cfg.h"


#ifdef __cplusplus
 extern "C" {
#endif


/* // for cortex m3
static inline
void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin >> 24)));
  MODIFY_REG(*pReg, ((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U)), (Mode << (POSITION_VAL(Pin) * 4U)));
} */

static inline
void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  MODIFY_REG(GPIOx->MODER, (3U << (POSITION_VAL(Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
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

static inline
void WriteOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask, bool state)
{
  if(state)
  {
    SetOutputPin(GPIOx, PinMask);
  }
  else
  {
    ResetOutputPin(GPIOx, PinMask);
  }
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

int16_t ugui_TOUCH_MeasureZ(void)
{
  // for X measurements
  GPIO_Z_MeasurementSetup();
  pressure = GPIO_Z_Measurement();
  return pressure;
}

int16_t ugui_TOUCH_Px(void)
{
  int16_t Px;
  Px = ((ugui_TOUCH_MeasureX() - TOUCH_AD_BOTTOM) * (YSIZE_PHYS-1) / (TOUCH_AD_TOP - TOUCH_AD_BOTTOM));
  if (Px < 0) Px = 0;
  if (Px > YSIZE_PHYS) Px = YSIZE_PHYS;
  return YSIZE_PHYS - Px; // reverse Px
}

/* ***** attention! ***************************************************
 * x and y turned 90° to right
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

int16_t ugui_TOUCH_Py(void)
{
  int16_t Py;
  Py = ((ugui_TOUCH_MeasureY()) - TOUCH_AD_RIGHT) * (XSIZE_PHYS-1) / (TOUCH_AD_LEFT - TOUCH_AD_RIGHT);
  if (Py < 0) Py = 0;
  if (Py > XSIZE_PHYS) Py = XSIZE_PHYS;
  return Py;
}

uint8_t ugui_TOUCH_StateZ(void)
{
  pressure = ugui_TOUCH_MeasureZ();
  if(pressure < MAX_PRESSURE)
  return TOUCH_STATE_PRESSED;
  else return TOUCH_STATE_RELEASED;
}

void GPIO_X_MeasurementSetup(void)
{
  // turn off the display
  SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  // Y axis YP-analog YM-disconnect
  SetPinMode(YM_GPIO_Port, YM_Pin, MODE_INPUT);
  SetPinMode(YP_GPIO_Port, YP_Pin, MODE_ANALOG);
  // X axis output
  SetPinMode(XM_GPIO_Port, XM_Pin, MODE_OUTPUT);
  SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);
  // X axis connected
  WriteOutputPin(XP_GPIO_Port, XP_Pin, 1);
  WriteOutputPin(XM_GPIO_Port, XM_Pin, 0);
}

uint16_t GPIO_X_Measurement(void)
{
  uint16_t xValue;
  xValue = GPIO_Measurement(ADC1, YP_GPIO_Port, YP_Pin);
  GPIO_Restore_Outputs();
  return xValue;
}

void GPIO_Y_MeasurementSetup(void)
{
  // turn off the display
  SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  // X axis XM-analog XP-disconnect
  SetPinMode(XM_GPIO_Port, XM_Pin, MODE_ANALOG);
  SetPinMode(XP_GPIO_Port, XP_Pin, MODE_INPUT);
  // Y axis output
  SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
  SetPinMode(YP_GPIO_Port, YP_Pin, MODE_OUTPUT);
  // Y axis connected
  WriteOutputPin(YP_GPIO_Port, YP_Pin, 1);
  WriteOutputPin(YM_GPIO_Port, YM_Pin, 0);
}

uint16_t GPIO_Y_Measurement(void)
{
  uint16_t yValue;
  yValue = GPIO_Measurement(ADC1, XM_GPIO_Port, XM_Pin);
  GPIO_Restore_Outputs();
  return yValue;
}

void GPIO_Z_MeasurementSetup(void)
{
  // turn off the display
  SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  // XM-analog YP-analog
  SetPinMode(XM_GPIO_Port, XM_Pin, MODE_ANALOG);
  SetPinMode(YP_GPIO_Port, YP_Pin, MODE_ANALOG);
  // XP output YM output
  SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);
  SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
  // XP YM connect
  WriteOutputPin(XP_GPIO_Port, XP_Pin, 0);
  WriteOutputPin(YM_GPIO_Port, YM_Pin, 1);
}

int16_t GPIO_Z_Measurement(void)
{
  int16_t pressure;
  // --
  uint16_t z1 = GPIO_Measurement(ADC1, XM_GPIO_Port, XM_Pin);
  uint16_t z2 = GPIO_Measurement(ADC1, YP_GPIO_Port, YP_Pin);
  // --
  /* printf("z1: %i\t", z1);
  printf("z1: %i\t", z2);*/
  pressure = abs((4096 + z2 - z1) / 2);
  // printf("pressure: %i\n", pressure);
  GPIO_Restore_Outputs();
  return pressure;
}

uint16_t GPIO_Measurement(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    LL_mDelay(1);
    uint32_t Value = 0;
    for (uint8_t i = 0; i < 64; i++) // (65535/4095) == 16 max for uint16_t
    {
      Value += adc_get_value(ADCx, GPIOx, Pin);
    }
    GPIO_Restore_Outputs();
    return Value / 64;
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

