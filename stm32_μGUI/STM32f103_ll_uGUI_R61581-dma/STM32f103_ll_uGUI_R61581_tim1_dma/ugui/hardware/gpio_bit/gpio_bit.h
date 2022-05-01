#ifndef __gpio_bit_H
#define __gpio_bit_H

#include "stdint.h"
#include "stdbool.h"

#include "stm32f103xb.h"

/*
 * GPIO_Clear_Pin(GPIOx, 1 << pin_number);
 */
static inline void
GPIO_Clear_Pin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  if(GPIOx==GPIOA) (*(volatile uint32_t *)(0x40010814)) = GPIO_Pin;
  if(GPIOx==GPIOB) (*(volatile uint32_t *)(0x40010C14)) = GPIO_Pin;
  if(GPIOx==GPIOC) (*(volatile uint32_t *)(0x40011014)) = GPIO_Pin;
}

/*
 * GPIO_Set_Pin(GPIOx, 1 << pin_number);
 */
static inline void
GPIO_Set_Pin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  if(GPIOx==GPIOA)  (*(volatile uint32_t *)(0x40010810)) = GPIO_Pin;
  if(GPIOx==GPIOB)  (*(volatile uint32_t *)(0x40010C10)) = GPIO_Pin;
  if(GPIOx==GPIOC)  (*(volatile uint32_t *)(0x40011010)) = GPIO_Pin;
}

/*
 * GPIO_Toggle_Pin(GPIOx, 1 << pin_number);
 */
static inline void
GPIO_Toggle_Pin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  if(GPIOx==GPIOA) (*(volatile uint32_t *)(0x4001080C)) ^= GPIO_Pin;
  if(GPIOx==GPIOB) (*(volatile uint32_t *)(0x40010C0C)) ^= GPIO_Pin;
  if(GPIOx==GPIOC) (*(volatile uint32_t *)(0x4001100C)) ^= GPIO_Pin;
}

static inline void
GPIO_Write_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool BitVal)
{
  if(BitVal)
  {
    GPIO_Set_Pin(GPIOx, GPIO_Pin);
  }
  else
  {
    GPIO_Clear_Pin(GPIOx, GPIO_Pin);
  }
}

/*
 * GPIO_Read_Pin(GPIOx, 1 << pin_number);
 */
static inline uint8_t
GPIO_Read_Pin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;

  if ((GPIOx->IDR & GPIO_Pin) != 0)
  {
    bitstatus = 1;
  }
  else
  {
    bitstatus = 0;
  }
  return bitstatus;
}

#endif // __gpio_bit_H

