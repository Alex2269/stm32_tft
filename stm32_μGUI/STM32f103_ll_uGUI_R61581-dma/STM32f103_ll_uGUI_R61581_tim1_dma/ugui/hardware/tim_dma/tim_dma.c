#include "tim_dma.h"
#include "stm32f1xx_ll_dma_flags.h"

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
  if(TIMx == TIM1)
  {
    LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM1);
    LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM1);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  }
  if(TIMx == TIM2)
  {
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  }
  if(TIMx == TIM3)
  {
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM3);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM3);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  }
  if(TIMx == TIM4)
  {
    LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM4);
    LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM4);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  }

  LL_TIM_DisableCounter(TIMx);
  LL_TIM_SetPrescaler(TIMx, 0);
  LL_TIM_SetCounterMode(TIMx, LL_TIM_COUNTERMODE_UP);
  LL_TIM_EnableARRPreload(TIMx);
  // LL_TIM_EnableAllOutputs(TIMx);
  LL_TIM_SetAutoReload(TIMx, 1);
  LL_TIM_SetClockDivision(TIMx, LL_TIM_CLOCKDIVISION_DIV1);
  LL_TIM_DisableARRPreload(TIMx);
  LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIMx);
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

void DMA_Transfer_Complete(DMA_TypeDef *DMAx, uint32_t Channel)
{
  #define TIMx TIM1
  /* Disable timer counter. */
  LL_TIM_DisableCounter(TIMx);

  /* Check if the DMA Channel is effectively disabled */
  LL_DMA_DisableChannel(DMAx, Channel);
  // while(LL_DMA_IsEnabledChannel(DMAx, Channel)) delay_us(1);

  // LL_DMA_ClearFlag_HT(DMAx, Channel); // Clear Channel x half transfer flag.
  // LL_DMA_EnableIT_HT(DMAx, Channel); // Enable Half transfer interrupt.
  LL_DMA_ClearFlag_TC(DMAx, Channel); // Clear Channel x transfer complete flag.
  LL_DMA_EnableIT_TC(DMAx, Channel); // Enable Transfer complete interrupt.
  LL_DMA_ClearFlag_TE(DMAx, Channel); // Clear Channel x transfer error flag.
  LL_DMA_EnableIT_TE(DMAx, Channel); // Enable Transfer error interrupt.

  // LL_DMA_DisableIT_DME(DMAx, Channel); // Disable Direct mode error interrupt.
  // LL_DMA_EnableIT_DME(DMAx, Channel); // Enable Direct mode error interrupt.
}

void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Channel, uint32_t DataLength)
{
  /* Check if the DMA Channel is effectively disabled */
  LL_DMA_DisableChannel(DMAx, Channel);
  // while(LL_DMA_IsEnabledChannel(DMAx, Channel)) delay_us(1);

  // LL_DMA_ClearFlag_GI(DMAx, Channel); // Clear Channel 1 global interrupt flag.
  LL_DMA_ClearFlag_TC(DMAx, Channel); // Clear Channel 1 transfer complete flag.
  // LL_DMA_ClearFlag_HT(DMAx, Channel); // Clear Channel 1 half transfer flag.
  LL_DMA_ClearFlag_TE(DMAx, Channel); // Clear Channel 1 transfer error flag.

  LL_DMA_SetDataLength(DMAx, Channel, DataLength);
  LL_DMA_SetMemoryAddress(DMAx, Channel, SrcAddress);
  LL_DMA_SetPeriphAddress(DMAx, Channel, DstAddress);
  LL_DMA_SetDataTransferDirection(DMAx, Channel, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  // LL_DMA_SetRegiter_CR(DMAx, Channel, dma2_CR_register_save);

  #define TIMx TIM1
  LL_TIM_EnableCounter(TIMx);
  LL_DMA_EnableChannel(DMAx, Channel);
}

// void DMA1_Channel5_IRQHandler(void) { DMA_Transfer_Complete(DMA1, 5); } /* Clear Channel x transfer complete flag. */

