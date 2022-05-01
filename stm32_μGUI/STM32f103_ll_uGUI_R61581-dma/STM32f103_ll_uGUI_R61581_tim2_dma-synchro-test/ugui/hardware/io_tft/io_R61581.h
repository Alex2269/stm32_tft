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

extern __align(2) uint16_t dma_buffer[];
extern uint16_t dma_count;

static inline void
tft_write_reg(uint8_t eightbit)
{
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
  GPIOB->ODR = control | (eightbit<<8);
  for(uint16_t i=0;i<100;i++) __asm volatile ("nop");
  // GPIOB->ODR = control | (eightbit<<8) | (1<<6);
  // LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
}

static inline void
tft_write_data(uint8_t eightbit)
{
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
  GPIOB->ODR = control | (eightbit<<8);
  for(uint16_t i=0;i<100;i++) __asm volatile ("nop");
  // GPIOB->ODR = control | (eightbit<<8) | (1<<6);
  // LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
}

static inline void
write_to_buffer_reg(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
  dma_buffer[dma_count] = control | (eightbit<<8); dma_count++;
  // dma_buffer[dma_count] = control | (eightbit<<8) | (1<<6); dma_count++;
}

static inline void
write_to_buffer_data(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
  dma_buffer[dma_count] = control | (eightbit<<8); dma_count++;
  // dma_buffer[dma_count] = control | (eightbit<<8) | (1<<6); dma_count++;

  if(dma_count>48)
  {
    write_dma_buffer_data(dma_count);
    dma_count = 0;
  }
}

void write_dma_buffer_data(uint16_t count);

// void write_to_buffer_reg(uint8_t eightbit);
// void write_to_buffer_data(uint8_t eightbit);

#pragma GCC pop_options

#endif // io_R61581_H_

/* [] END OF FILE */

