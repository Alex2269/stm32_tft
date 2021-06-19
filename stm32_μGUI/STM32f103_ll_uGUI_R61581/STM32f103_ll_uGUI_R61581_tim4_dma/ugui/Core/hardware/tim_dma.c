#include "tim_dma.h"

void dma_clock_irq(uint32_t Channel)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* DMA1 interrupt init */
  /* DMA1 Channel IRQn interrupt configuration */
  NVIC_SetPriority(Channel+10, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(Channel+10);
}

void timer_setting(TIM_TypeDef *TIMx)
{
  LL_APB1_GRP1_EnableClock(RCC_APB1ENR_TIM4EN);
  LL_TIM_DisableCounter(TIMx);
  LL_TIM_SetPrescaler(TIMx, 0);
  LL_TIM_EnableARRPreload(TIMx);
  // LL_TIM_EnableAllOutputs(TIMx);
  LL_TIM_SetAutoReload(TIMx, 1);
  LL_TIM_EnableCounter(TIMx);
}

void dma_setting(DMA_TypeDef *DMAx, uint32_t Channel)
{
  LL_DMA_SetDataTransferDirection(DMAx, Channel, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetChannelPriorityLevel(DMAx, Channel, LL_DMA_PRIORITY_VERYHIGH);
  LL_DMA_SetMode(DMAx, Channel, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMAx, Channel, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMAx, Channel, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMAx, Channel, LL_DMA_PDATAALIGN_HALFWORD);
  LL_DMA_SetMemorySize(DMAx, Channel, LL_DMA_MDATAALIGN_HALFWORD);
}

void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Channel, uint32_t DataLength)
{
  LL_DMA_DisableChannel(DMAx, Channel);

  DMAx->IFCR |= 2<<(4*(Channel-1)); // Channel transfer complete flag
  DMAx->IFCR |= 4<<(4*(Channel-1)); // Channel transfer error flag

  LL_DMA_SetDataLength(DMAx, Channel, DataLength);
  LL_DMA_ConfigAddresses(DMAx, Channel, (uint32_t)SrcAddress, (uint32_t)DstAddress, LL_DMA_GetDataTransferDirection(DMAx, Channel));

  LL_DMA_EnableIT_TC(DMAx, Channel); // Enable Transfer complete interrupt
  LL_DMA_EnableIT_TE(DMAx, Channel); // Enable Transfer error interrupt
  LL_DMA_EnableChannel(DMAx, Channel);
}

