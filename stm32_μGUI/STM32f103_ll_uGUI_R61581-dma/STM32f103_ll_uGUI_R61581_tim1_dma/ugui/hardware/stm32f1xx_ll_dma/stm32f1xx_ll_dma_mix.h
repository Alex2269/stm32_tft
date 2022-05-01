#ifndef __STM32F1XX_LL_DMA_MIX_H
#define __STM32F1XX_LL_DMA_MIX_H

#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_dma_flags.h"

/* Set Configuration register CCR. */
static inline
void LL_DMA_SetRegiter_CCR(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t Configuration)
{
  if(Channel > 7) return;
  WRITE_REG(((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR, Configuration);
}

/* Get Configuration register CCR. */
static inline
uint32_t LL_DMA_GetRegiter_CCR(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;
  return READ_REG(((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR);
}

#endif // __STM32F1XX_LL_DMA_MIX_H
