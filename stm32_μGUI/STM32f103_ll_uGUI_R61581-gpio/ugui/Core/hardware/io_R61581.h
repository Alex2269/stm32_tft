/***
 * 
 * io_R61581.h
 *
***/

#ifndef io_R61581_H_
#define io_R61581_H_

#include "stm32f103xb.h"
#include "ugui_R61581.h"
#include "main.h"

#pragma GCC push_options
#pragma GCC optimize("O3")

static inline void
tft_write_reg(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
  GPIOB->ODR = control | (eightbit<<8);
  LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

static inline void
tft_write_data(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
  GPIOB->ODR = control | (eightbit<<8);
  LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

#pragma GCC pop_options

#endif // io_R61581_H_

/* [] END OF FILE */

