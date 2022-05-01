/***
 * 
 * io_R61581.h
 *
***/

#ifndef io_R61581_H_
#define io_R61581_H_

#include "stm32f401xc.h"
#include "ugui_R61581.h"
#include "main.h"

#pragma GCC push_options
#pragma GCC optimize("O3")

extern __align(2) uint16_t dma_buffer[];
extern uint16_t dma_count;

/*
 * due to the lack of contact PB11 you have to use PB2
 */
static inline void
tft_write_reg(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000011 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
  static uint16_t data;
  static uint16_t th;
  data = control | (eightbit<<8);

  uint16_t t = data & 0b0000100000000000; // Pull out the 11-th bit.
  t >>= 9; // Left shift the shift-th bit.
  data          =  data & 0b1111011111111111; // Clear the 11-th bit
  data |= t;

  GPIOB->ODR = data;
  delay_us(1);
  GPIOB->ODR = data | (1<<6);
  // LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

static inline void
tft_write_data(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000011 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
  static uint16_t data;
  static uint16_t th;
  data = control | (eightbit<<8);

  uint16_t t = data & 0b0000100000000000; // Pull out the 11-th bit.
  t >>= 9; // Left shift the shift-th bit.
  data          =  data & 0b1111011111111111; // Clear the 11-th bit
  data |= t;

  GPIOB->ODR = data;
  delay_us(1);
  GPIOB->ODR = data | (1<<6);
  // LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
}

static inline void
write_to_buffer_reg(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000011 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
  static uint16_t data;
  static uint16_t th;
  data = control | (eightbit<<8);

  uint16_t t = data & 0b0000100000000000; // Pull out the 11-th bit.
  t >>= 9; // Left shift the shift-th bit.
  data          =  data & 0b1111011111111111; // Clear the 11-th bit
  data |= t;

  dma_buffer[dma_count] = data; dma_count++;
  dma_buffer[dma_count] = data | (1<<6); dma_count++;
}

static inline void
write_to_buffer_data(uint8_t eightbit)
{
  static uint16_t control = 0b0000000000000011 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
  static uint16_t data;
  static uint16_t th;
  data = control | (eightbit<<8);

  uint16_t t = data & 0b0000100000000000; // Pull out the 11-th bit.
  t >>= 9; // Left shift the shift-th bit.
  data          =  data & 0b1111011111111111; // Clear the 11-th bit
  data |= t;

  dma_buffer[dma_count] = data; dma_count++;
  dma_buffer[dma_count] = data | (1<<6); dma_count++;

  if(dma_count>96)
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

