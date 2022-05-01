/***
 * 
 * io_SSD1963.с
 *
***/

#include "io_SSD1963.h"

#pragma GCC push_options
#pragma GCC optimize("O3")

__align(2) uint16_t dma_buffer[50];

void tft_write_data_com(uint8_t eightbit, uint8_t dc_pin)
{
  static uint16_t control = 0b0000100000000011 | (1<<RD) | (0<<WR) | (0<<DC) | (0<<CS) | (1<<RST);

  if(dc_pin)
    GPIOB->ODR = control | (eightbit<<2) | (1<<DC);
  else
    GPIOB->ODR = control | (eightbit<<2);

  GPIOB->ODR = control | (eightbit<<2) | (1<<WR);
}

void write_to_buffer_data_com(uint8_t eightbit, uint8_t dc_pin)
{
  static uint16_t dma_count;
  static uint16_t control = 0b0000100000000011 | (1<<RD) | (0<<WR) | (0<<DC) | (0<<CS) | (1<<RST);

  if(dc_pin)
    dma_buffer[dma_count] = control | (eightbit<<2) | (1<<DC);
  else
    dma_buffer[dma_count] = control | (eightbit<<2);
  dma_count++;

  dma_buffer[dma_count] = control | (eightbit<<2) | (1<<WR);
  dma_count++;

  if(dma_count>49)
  {
    write_dma_buffer_data(dma_count);
    dma_count = 0;
  }
}

void write_dma_buffer_data(uint16_t count)
{
  #define Stream 5
  #define TIMx TIM1
  #define DMAx DMA2
  dma_transmit(DMAx, (uint32_t)dma_buffer, ((uint32_t)&GPIOB->ODR), Stream, count);
  TIMx->DIER |= TIM_DIER_UDE; // LL_TIM_EnableDMAReq_UPDATE(TIMx);
}

#pragma GCC pop_options

/* [] END OF FILE */
