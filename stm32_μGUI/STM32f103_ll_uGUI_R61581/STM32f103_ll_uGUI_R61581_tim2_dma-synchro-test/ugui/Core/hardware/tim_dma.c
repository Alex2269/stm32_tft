#include "tim_dma.h"

void dma_clock_irq(uint32_t Channel)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* DMA1 interrupt init */
  /* DMA1 Channel IRQn interrupt configuration */
  NVIC_SetPriority(Channel+10, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(Channel+10);
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

void timer_setting(TIM_TypeDef *TIMx)
{
  LL_APB1_GRP1_EnableClock(RCC_APB1ENR_TIM2EN);
  LL_TIM_DisableCounter(TIMx);
  LL_TIM_SetPrescaler(TIMx, 0);
  LL_TIM_EnableARRPreload(TIMx);
  WRITE_REG(TIMx->PSC, 0); // LL_TIM_SetPrescaler(TIMx, 0);
  WRITE_REG(TIMx->ARR, 22-1); // LL_TIM_SetAutoReload(TIMx, 36-1); // period
  WRITE_REG(TIMx->CCR1, 2); // LL_TIM_OC_SetCompareCH1(TIMx, 18); // 50% duty cycle

  NVIC_SetPriority(TIM2_IRQn, 1);
}

// output synchro clock on Channel
void timer_synchro_clock_out(TIM_TypeDef *TIMx, uint32_t Channel)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE); // PA0 the timer2 ch1 pin
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  SET_BIT(TIMx->CCER, TIM_CCER_CC1E); // LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH1);
  LL_TIM_OC_SetPolarity(TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH); // LOW/HIGH
  LL_TIM_OC_SetMode(TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1); // LL_TIM_OCMODE_TOGGLE
  MODIFY_REG(TIMx->CR1, (TIM_CR1_DIR | TIM_CR1_CMS), LL_TIM_COUNTERMODE_UP); // LL_TIM_SetCounterMode(TIMx, LL_TIM_COUNTERMODE_UP);
  SET_BIT(TIMx->DIER, TIM_DIER_CC1IE); // LL_TIM_EnableIT_CC1(TIMx);
  SET_BIT(TIMx->CR1, TIM_CR1_CEN); // LL_TIM_EnableCounter(TIMx);
  SET_BIT(TIMx->EGR, TIM_EGR_UG); // LL_TIM_GenerateEvent_UPDATE(TIM_TypeDef *TIMx)
}

// pin clocking for dma data
// output synchro clock on Channel
void timer_synchro_clock_out_start(TIM_TypeDef *TIMx, uint32_t Channel)
{
  TIMx->CCER |= Channel;  /*!<Capture/Compare CHx output enable */
}

void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Channel, uint32_t DataLength)
{
  LL_DMA_DisableChannel(DMAx, Channel);

  DMAx->IFCR |= 2<<(4*(Channel-1)); // Channel Transfer Complete flag
  // DMAx->IFCR |= 4<<(4*(Channel-1)); // Channel Half Transfer flag
  DMAx->IFCR |= 8<<(4*(Channel-1)); // Channel Transfer Error flag

  LL_DMA_SetDataLength(DMAx, Channel, DataLength);
  LL_DMA_ConfigAddresses(DMAx, Channel, (uint32_t)SrcAddress, (uint32_t)DstAddress, LL_DMA_GetDataTransferDirection(DMAx, Channel));

  timer_synchro_clock_out_start(TIM2, LL_TIM_CHANNEL_CH1);
  LL_DMA_EnableIT_TC(DMAx, Channel); // Enable Transfer complete interrupt
  LL_DMA_EnableIT_TE(DMAx, Channel); // Enable Transfer error interrupt

  LL_DMA_EnableChannel(DMAx, Channel);
}
