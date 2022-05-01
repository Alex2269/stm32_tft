/***
 * 
 * io_ILI9325.h
 *
***/

#ifndef io_ILI9325_H_
#define io_ILI9325_H_

#include "stm32f103xb.h"
#include "ugui_ILI9325.h"
#include "stm32_tick.h"

#include "main.h"

#pragma GCC push_options
#pragma GCC optimize("O3")

static inline void
display_off(void)
{
  LL_GPIO_SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
}

static inline void
display_on(void)
{
  LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
}

static inline void
display_reset(void)
{
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(5);
  LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(15);
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(15);
  LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  LL_GPIO_SetOutputPin(LCD_RD_GPIO_Port, LCD_RD_Pin);
}

static inline void
tft_write_reg(uint8_t eightbit)
{
    LL_GPIO_SetOutputPin(LCD_RD_GPIO_Port, LCD_RD_Pin); // idle = high
    LL_GPIO_ResetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin); // low = command
    GPIOA->ODR = (eightbit >> 2) | (eightbit << 6);
    LL_GPIO_ResetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin);
    LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

static inline void
tft_write_data(uint8_t eightbit)
{
    LL_GPIO_SetOutputPin(LCD_RD_GPIO_Port, LCD_RD_Pin); // idle = high
    LL_GPIO_SetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin); // high = data
    GPIOA->ODR = (eightbit >> 2) | (eightbit << 6);
    LL_GPIO_ResetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin);
    LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

#pragma GCC pop_options

#endif // io_ILI9325_H_

/* [] END OF FILE */

