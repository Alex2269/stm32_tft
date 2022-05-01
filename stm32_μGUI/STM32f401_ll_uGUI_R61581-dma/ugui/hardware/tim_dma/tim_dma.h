#ifndef __tim_dma__h
#define __tim_dma__h

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
void hw_tim_start(TIM_TypeDef *TIMx)
{
  TIMx->CNT=0;
  TIMx->BDTR |= TIM_BDTR_MOE;
  TIMx->CR1  |= TIM_CR1_CEN;
}

void hw_pwm_chan_start(TIM_TypeDef *TIMx, uint32_t Channel)
{
  TIMx->CCER |= (uint32_t)0x01 << ( (Channel - 1) * 4 );
}

void hw_pwm_chan_stop(TIM_TypeDef *TIMx, uint32_t Channel)
{
  TIMx->CCER &= ~((uint32_t)0x01 << ( (Channel - 1) * 4 ));
}
*/

static inline
void TIM_EnableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Enable capture/compare TIMx interrupt.
{
  TIMx->DIER |= (uint32_t)0x01 << Interrupt;
}

static inline
void TIM_DisableIT(TIM_TypeDef *TIMx, uint32_t Interrupt) // Disable capture/compare TIMx interrupt.
{
  TIMx->DIER &= ~(uint32_t)0x01 << Interrupt;
}

static inline
void TIM_ClearFlag_CC(TIM_TypeDef *TIMx, uint32_t CCxFlag) // Clear the capture/compare TIMx interrupt flag
{
  TIMx->SR &= ~(uint32_t)0x01 << CCxFlag;
}

/*
 * Indicate whether Capture/Compare interrupt flag
 * is set (Capture/Compare flag interrupt is pending).
 */
static inline
uint32_t TIM_IsActiveFlag_CC(TIM_TypeDef *TIMx, uint32_t flag)
{
  return ((READ_BIT(TIMx->SR, flag) == (flag)) ? 1UL : 0UL);
}

void dma_clock_irq(uint32_t Channel);
void dma_setting(DMA_TypeDef *DMAx, uint32_t Channel);
void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Channel, uint32_t DataLength);

#ifdef __cplusplus
}
#endif

#endif // __tim_dma__h
