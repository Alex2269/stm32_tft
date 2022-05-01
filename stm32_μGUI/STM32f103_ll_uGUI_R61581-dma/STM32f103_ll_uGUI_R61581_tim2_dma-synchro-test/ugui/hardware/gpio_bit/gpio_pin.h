#ifndef __gpio_pin_H
#define __gpio_pin_H

#include "stdint.h"
#include "stdbool.h"

#include "stm32f103xb.h"

/*
 * GPIO_Pin_Clear(GPIOx, pin number);
 */
static inline void
GPIO_Pin_Clear(GPIO_TypeDef* GPIOx, uint32_t Pin_number)
{
  Pin_number = (1<<Pin_number);
  if(GPIOx==GPIOA) (*(volatile uint32_t *)(0x40010814)) = Pin_number;
  if(GPIOx==GPIOB) (*(volatile uint32_t *)(0x40010C14)) = Pin_number;
  if(GPIOx==GPIOC) (*(volatile uint32_t *)(0x40011014)) = Pin_number;
}

/*
 * GPIO_Pin_Set(GPIOx, pin number);
 */
static inline void
GPIO_Pin_Set(GPIO_TypeDef* GPIOx, uint32_t Pin_number)
{
  Pin_number = (1<<Pin_number);
  if(GPIOx==GPIOA)  (*(volatile uint32_t *)(0x40010810)) = Pin_number;
  if(GPIOx==GPIOB)  (*(volatile uint32_t *)(0x40010C10)) = Pin_number;
  if(GPIOx==GPIOC)  (*(volatile uint32_t *)(0x40011010)) = Pin_number;
}

/*
 * GPIO_Pin_Toggle(GPIOx, pin number);
 */
static inline void
GPIO_Pin_Toggle(GPIO_TypeDef* GPIOx, uint32_t Pin_number)
{
  Pin_number = (1<<Pin_number);
  if(GPIOx==GPIOA) (*(volatile uint32_t *)(0x4001080C)) ^= Pin_number;
  if(GPIOx==GPIOB) (*(volatile uint32_t *)(0x40010C0C)) ^= Pin_number;
  if(GPIOx==GPIOC) (*(volatile uint32_t *)(0x4001100C)) ^= Pin_number;
}

static inline void
GPIO_Pin_Write(GPIO_TypeDef* GPIOx, uint16_t Pin_number, bool BitVal)
{
  if(BitVal)
  {
    GPIO_Pin_Set(GPIOx, Pin_number);
  }
  else
  {
    GPIO_Pin_Clear(GPIOx, Pin_number);
  }
}

/*
 * GPIO_Pin_Read(GPIOx, pin number);
 */
static inline uint8_t
GPIO_Pin_Read(GPIO_TypeDef* GPIOx, uint16_t Pin_number)
{
  Pin_number = (1<<Pin_number);
  uint8_t bitstatus = 0x00;

  if ((GPIOx->IDR & Pin_number) != 0)
  {
    bitstatus = 1;
  }
  else
  {
    bitstatus = 0;
  }
  return bitstatus;
}

// floating input
static inline void
GPIO_Pin_Init_Input(GPIO_TypeDef * GPIOx, uint8_t Pin_number)
{
  if (GPIOx == GPIOA)
  (RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
  else if (GPIOx == GPIOB)
  (RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
  else if (GPIOx == GPIOC)
  (RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
  if (Pin_number > 7)
  {
    // erase the interested area
    GPIOx->CRH = GPIOx->CRH & ~(0xF << (Pin_number - 8) * 4);
    // Register new values
    GPIOx->CRH = GPIOx->CRH |(0x4 << (Pin_number - 8) * 4);
  }
  else
  {
    // erase the interested area
    GPIOx->CRL = GPIOx->CRL & ~(0xF << Pin_number * 4);
    // Register new values
    GPIOx->CRL = GPIOx->CRL |(0x4 << Pin_number * 4);
  }
}

#endif // __gpio_pin_H

