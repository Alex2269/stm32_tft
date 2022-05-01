#ifndef __STM32F1xx_LL_DMA_FLAGS_H
#define __STM32F1xx_LL_DMA_FLAGS_H

#include "stm32f1xx_ll_dma.h"

/* Clear Channel x global interrupt flag. */
static inline
void LL_DMA_ClearFlag_GI(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_IFCR_CGIF1, DMA_IFCR_CGIF2, DMA_IFCR_CGIF3,
    DMA_IFCR_CGIF4, DMA_IFCR_CGIF5, DMA_IFCR_CGIF6, DMA_IFCR_CGIF7
  };

  WRITE_REG(DMAx->IFCR , flag_t[Channel]);
}

/* Clear Channel x half transfer flag. */
static inline
void LL_DMA_ClearFlag_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_IFCR_CHTIF1, DMA_IFCR_CHTIF2, DMA_IFCR_CHTIF3,
    DMA_IFCR_CHTIF4, DMA_IFCR_CHTIF5, DMA_IFCR_CHTIF6, DMA_IFCR_CHTIF7
  };

  WRITE_REG(DMAx->IFCR , flag_t[Channel]);
}

/* Clear Channel x transfer complete flag. */
static inline
void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_IFCR_CTCIF1, DMA_IFCR_CTCIF2, DMA_IFCR_CTCIF3,
    DMA_IFCR_CTCIF4, DMA_IFCR_CTCIF5, DMA_IFCR_CTCIF6, DMA_IFCR_CTCIF7
  };

  WRITE_REG(DMAx->IFCR , flag_t[Channel]);
}

/* Clear Channel x transfer error flag. */
static inline
void LL_DMA_ClearFlag_TE(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_IFCR_CTEIF1, DMA_IFCR_CTEIF2, DMA_IFCR_CTEIF3,
    DMA_IFCR_CTEIF4, DMA_IFCR_CTEIF5, DMA_IFCR_CTEIF6, DMA_IFCR_CTEIF7
  };

  WRITE_REG(DMAx->IFCR , flag_t[Channel]);
}

/* Clear Channel x direct mode error flag. */
// static inline
// void LL_DMA_ClearFlag_DME(DMA_TypeDef *DMAx, uint32_t Channel)
// {
//   if(Channel > 7) return;
//
//   uint32_t flag_t[8] =
//   {
//     0, DMA_IFCR_CDMEIF1, DMA_IFCR_CDMEIF2, DMA_IFCR_CDMEIF3,
//     DMA_IFCR_CDMEIF4, DMA_IFCR_CDMEIF5, DMA_IFCR_CDMEIF6, DMA_IFCR_CDMEIF7
//   };
//
//   WRITE_REG(DMAx->IFCR , flag_t[Channel]);
// }

/* Clear Channel x FIFO error flag. */
// static inline
// void LL_DMA_ClearFlag_FE(DMA_TypeDef *DMAx, uint32_t Channel)
// {
//   if(Channel > 7) return;
//
//   uint32_t flag_t[8] =
//   {
//     0, DMA_IFCR_CFEIF1, DMA_IFCR_CFEIF2, DMA_IFCR_CFEIF3,
//     DMA_IFCR_CFEIF4, DMA_IFCR_CFEIF5, DMA_IFCR_CFEIF6, DMA_IFCR_CFEIF7
//   };
//
//   WRITE_REG(DMAx->IFCR , flag_t[Channel]);
// }

/* Get Channel x global interrupt flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_GI(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_ISR_GIF1, DMA_ISR_GIF2, DMA_ISR_GIF3,
    DMA_ISR_GIF4, DMA_ISR_GIF5, DMA_ISR_GIF6, DMA_ISR_GIF7
  };

  uint32_t flag = flag_t[Channel];

  return (READ_BIT(DMAx->ISR ,flag)==(flag));
}

/* Get Channel x half transfer flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_ISR_HTIF1, DMA_ISR_HTIF2, DMA_ISR_HTIF3,
    DMA_ISR_HTIF4, DMA_ISR_HTIF5, DMA_ISR_HTIF6, DMA_ISR_HTIF7
  };

  uint32_t flag = flag_t[Channel];

  return (READ_BIT(DMAx->ISR ,flag)==(flag));
}

/* Get Channel x transfer complete flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_ISR_TCIF1, DMA_ISR_TCIF2, DMA_ISR_TCIF3,
    DMA_ISR_TCIF4, DMA_ISR_TCIF5, DMA_ISR_TCIF6, DMA_ISR_TCIF7
  };

  uint32_t flag = flag_t[Channel];

  return (READ_BIT(DMAx->ISR ,flag)==(flag));
}

/* Get Channel x transfer error flag. */
static inline
uint32_t LL_DMA_IsActiveFlag_TE(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if(Channel > 7) return;

  uint32_t flag_t[8] =
  {
    0, DMA_ISR_TEIF1, DMA_ISR_TEIF2, DMA_ISR_TEIF3,
    DMA_ISR_TEIF4, DMA_ISR_TEIF5, DMA_ISR_TEIF6, DMA_ISR_TEIF7
  };

  uint32_t flag = flag_t[Channel];

  return (READ_BIT(DMAx->ISR ,flag)==(flag));
}

/* Get Channel x direct mode error flag. */
// static inline
// uint32_t LL_DMA_IsActiveFlag_DME(DMA_TypeDef *DMAx, uint32_t Channel)
// {
//   if(Channel > 7) return;
//
//   uint32_t flag_t[8] =
//   {
//     0, DMA_ISR_DMEIF1, DMA_ISR_DMEIF2, DMA_ISR_DMEIF3,
//     DMA_ISR_DMEIF4, DMA_ISR_DMEIF5, DMA_ISR_DMEIF6, DMA_ISR_DMEIF7
//   };
//
//   uint32_t flag = flag_t[Channel];
//
//   return (READ_BIT(DMAx->ISR ,flag)==(flag));
// }

/* Get Channel x FIFO error flag. */
// static inline
// uint32_t LL_DMA_IsActiveFlag_FE(DMA_TypeDef *DMAx, uint32_t Channel)
// {
//   if(Channel > 7) return;
//
//   uint32_t flag_t[8] =
//   {
//     0, DMA_ISR_FEIF1, DMA_ISR_FEIF2, DMA_ISR_FEIF3,
//     DMA_ISR_FEIF4, DMA_ISR_FEIF5, DMA_ISR_FEIF6, DMA_ISR_FEIF7
//   };
//
//   uint32_t flag = flag_t[Channel];
//
//   return (READ_BIT(DMAx->ISR ,flag)==(flag));
// }


#endif // __STM32F1xx_LL_DMA_FLAGS_H
