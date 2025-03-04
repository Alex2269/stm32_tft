#include "tim_dma.h"
#include "stm32f4xx_ll_dma_flags.h"

void dma_clock_irq(uint32_t Stream)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* DMA1 interrupt init */
  /* DMA1 Stream IRQn interrupt configuration */
  NVIC_SetPriority(Stream+10, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(Stream+10);
}

void dma_setting(DMA_TypeDef *DMAx, uint32_t Stream)
{
  LL_FLASH_EnableInstCache();
  LL_FLASH_DisableDataCache();

  LL_DMA_SetStreamPriorityLevel(DMAx, Stream, LL_DMA_PRIORITY_VERYHIGH);
  LL_DMA_SetMode(DMAx, Stream, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMAx, Stream, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMAx, Stream, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMAx, Stream, LL_DMA_PDATAALIGN_HALFWORD);
  LL_DMA_SetMemorySize(DMAx, Stream, LL_DMA_MDATAALIGN_HALFWORD);
}

void DMA_Transfer_Complete(DMA_TypeDef *DMAx, uint32_t Stream)
{
  #define TIMx TIM1
  /* Disable timer counter. */
  LL_TIM_DisableCounter(TIMx);

  /* Check if the DMA Stream is effectively disabled */
  LL_DMA_DisableStream(DMAx, Stream);
  while(LL_DMA_IsEnabledStream(DMAx, Stream)) delay_us(1);

  // LL_DMA_ClearFlag_HT(DMAx, Stream); // Clear Stream x half transfer flag.
  // LL_DMA_EnableIT_HT(DMAx, Stream); // Enable Half transfer interrupt.
  LL_DMA_ClearFlag_TC(DMAx, Stream); // Clear Stream x transfer complete flag.
  LL_DMA_EnableIT_TC(DMAx, Stream); // Enable Transfer complete interrupt.
  LL_DMA_ClearFlag_TE(DMAx, Stream); // Clear Stream x transfer error flag.
  LL_DMA_EnableIT_TE(DMAx, Stream); // Enable Transfer error interrupt.
  // LL_DMA_DisableIT_DME(DMAx, Stream); // Disable Direct mode error interrupt.
  // LL_DMA_EnableIT_DME(DMAx, Stream); // Enable Direct mode error interrupt.
}

void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Stream, uint32_t DataLength)
{
  LL_DMA_ClearFlag_TC(DMAx, Stream); // Clear Stream x transfer complete flag.
  // LL_DMA_ClearFlag_HT(DMAx, Stream); // Clear Stream x half transfer flag.
  LL_DMA_ClearFlag_TE(DMAx, Stream); // Clear Stream x transfer error flag.

  LL_DMA_DisableDoubleBufferMode(DMAx, Stream);
  LL_DMA_SetDataLength(DMAx, Stream, DataLength);
  LL_DMA_SetMemoryAddress(DMAx, Stream, SrcAddress);
  LL_DMA_SetPeriphAddress(DMAx, Stream, DstAddress);
  LL_DMA_SetDataTransferDirection(DMAx, Stream, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  // LL_DMA_SetRegiter_CR(DMAx, Stream, dma2_CR_register_save);

  #define TIMx TIM1
  LL_TIM_EnableCounter(TIMx);

  LL_DMA_EnableStream(DMAx, Stream);
}

// void DMA2_Stream5_IRQHandler(void) { DMA_Transfer_Complete(DMA2, 5); }

