#ifndef __tim_dma__h
#define __tim_dma__h

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif
    
void dma_clock_irq(uint32_t Channel);
void dma_setting(DMA_TypeDef *DMAx, uint32_t Channel);
void timer_setting(TIM_TypeDef *TIMx);
void timer_synchro_clock_out(TIM_TypeDef *TIMx, uint32_t Channel);
void timer_synchro_clock_out_start(TIM_TypeDef *TIMx, uint32_t Channel);
void dma_transmit(DMA_TypeDef *DMAx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t Channel, uint32_t DataLength);

#ifdef __cplusplus
}
#endif

#endif // __tim_dma__h

