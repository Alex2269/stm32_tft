
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

// // for cortex m3
// static inline
// void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
// {
//   register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin >> 24)));
//   MODIFY_REG(*pReg, ((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U)), (Mode << (POSITION_VAL(Pin) * 4U)));
// }

// for cortex m4
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

uint16_t ugui_TOUCH_MeasureX(void)
{
   int value_x = 0;
   gpio_y_connection();
   value_x = GPIO_Measurement(ADC1, XP_GPIO_Port, XP_Pin);
   // printf("value_x: %i\t", value_x);
   return value_x;
}

uint16_t ugui_TOUCH_MeasureY(void)
{
   int value_y = 0;
   gpio_x_connection();
   value_y = GPIO_Measurement(ADC1, YM_GPIO_Port, YM_Pin);
   // printf("value_y: %i\t", value_y);
   return value_y;
}

void gpio_x_connection(void)
{
    // write_pin(PortB,CS,1);
    SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);

    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_ANALOG);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_INPUT);

    // X axis output
    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_OUTPUT);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_OUTPUT);

    SetOutputPin(XP_GPIO_Port, XP_Pin);
    ResetOutputPin(XM_GPIO_Port, XM_Pin);
}

void gpio_y_connection(void)
{
    // write_pin(PortB,CS,1);
    SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);

    SetPinMode(XM_GPIO_Port, XM_Pin, MODE_INPUT);
    SetPinMode(XP_GPIO_Port, XP_Pin, MODE_ANALOG);

    // *** Y axis output
    SetPinMode(YM_GPIO_Port, YM_Pin, MODE_OUTPUT);
    SetPinMode(YP_GPIO_Port, YP_Pin, MODE_OUTPUT);

    SetOutputPin(YP_GPIO_Port, YP_Pin);
    ResetOutputPin(YM_GPIO_Port, YM_Pin);
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
    return Value/64;
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
