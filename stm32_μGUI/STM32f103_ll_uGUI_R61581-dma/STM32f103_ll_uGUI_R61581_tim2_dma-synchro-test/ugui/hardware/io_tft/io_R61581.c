/***
 * 
 * io_R61581.h
 *
***/

#include "io_R61581.h"
#include "tim_dma.h"

__align(2) uint16_t dma_buffer[102] = {0};
uint16_t dma_count = 0;

#pragma GCC push_options
#pragma GCC optimize("O3")

void write_dma_buffer_data(uint16_t count)
{
  #define Channel 2
  #define TIMx TIM2
  dma_transmit(DMA1, (uint32_t)dma_buffer, ((uint32_t)&GPIOB->ODR), Channel, count);
  TIMx->DIER |= TIM_DIER_UDE; // LL_TIM_EnableDMAReq_UPDATE(TIMx);
}

#pragma GCC pop_options

/* [] END OF FILE */
