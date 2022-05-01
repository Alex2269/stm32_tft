#ifndef _board_io_H
#define _board_io_H

#include "main.h"


#define TOUCH_CLK_Pin LL_GPIO_PIN_0
#define TOUCH_CLK_GPIO_Port GPIOA
#define TOUCH_CS_Pin LL_GPIO_PIN_1
#define TOUCH_CS_GPIO_Port GPIOA
#define TOUCH_MISO_Pin LL_GPIO_PIN_2
#define TOUCH_MISO_GPIO_Port GPIOA
#define TOUCH_MOSI_Pin LL_GPIO_PIN_3
#define TOUCH_MOSI_GPIO_Port GPIOA


#define LOW 0
#define HIGH 1


/* // for cortex m3
static inline
void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin >> 24)));
  MODIFY_REG(*pReg, ((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U)), (Mode << (POSITION_VAL(Pin) * 4U)));
} */

static inline
void SetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Pull)
{
  MODIFY_REG(GPIOx->PUPDR, (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(Pin) * 2U)), (Pull << (POSITION_VAL(Pin) * 2U)));
}

static inline
void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  MODIFY_REG(GPIOx->MODER, (3U << (POSITION_VAL(Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
}

static inline
void pin_in(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  /* GPIO Ports Clock Enable */
  if(GPIOx==GPIOA) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  if(GPIOx==GPIOB) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  if(GPIOx==GPIOC) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  if(GPIOx==GPIOD) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  // LL_GPIO_SetPinSpeed(GPIOx, GPIO_Pin, LL_GPIO_SPEED_FREQ_LOW);
  // LL_GPIO_SetPinOutputType(GPIOx, GPIO_Pin, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinMode(GPIOx, GPIO_Pin, LL_GPIO_MODE_INPUT);
}

static inline
void pin_out(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  /* GPIO Ports Clock Enable */
  if(GPIOx==GPIOA) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  if(GPIOx==GPIOB) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  if(GPIOx==GPIOC) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  if(GPIOx==GPIOD) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  LL_GPIO_SetPinSpeed(GPIOx, GPIO_Pin, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinOutputType(GPIOx, GPIO_Pin, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinMode(GPIOx, GPIO_Pin, LL_GPIO_MODE_OUTPUT);
}

static inline
void SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  // WRITE_REG(GPIOx->BSRR, PinMask);
  SET_BIT(GPIOx->ODR, PinMask);
}

static inline
void ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  // WRITE_REG(GPIOx->BSRR, (PinMask << 16));
  CLEAR_BIT(GPIOx->ODR, PinMask);
}

static inline
void WriteOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask, int state)
{
  // pin_out(GPIOx, PinMask);

  if(state)
  {
    // GPIOx->ODR |= PinMask;
    SetOutputPin(GPIOx, PinMask);
  }
  else
  {
    // GPIOx->ODR &= ~PinMask;
    ResetOutputPin(GPIOx, PinMask);
  }
}

static inline
uint32_t ReadInputPort(GPIO_TypeDef *GPIOx)
{
  return (uint32_t)(READ_REG(GPIOx->IDR));
}

static inline
uint32_t IsInputPinSet(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (READ_BIT(GPIOx->IDR, PinMask) == (PinMask));
}

static inline
uint32_t ReadInputPin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  uint32_t bitstatus;

  if(GPIOx->IDR & GPIO_Pin)
  {
    bitstatus = 1;
  }
  else
  {
    bitstatus = 0;
  }
  return bitstatus;
}

static inline
void soft_spi_begin(void)
{
  pin_out(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin);
  pin_in(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
  pin_out(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin);
  pin_out(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);
  ResetOutputPin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);
}

static inline
void soft_spi_end(void)
{
  SetOutputPin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);
  pin_in(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin);
  pin_in(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
  pin_in(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin);
  // pin_in(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);
}


#endif // _board_io_H

