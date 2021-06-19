#ifndef TFT_IO_HEADER_H
#define TFT_IO_HEADER_H

/******************** TFT-480-QVT-9481x tft LIBRARY ***********************
******** Description : This is library for TFT-9481x using STM32F103C8T6 uC 
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stdbool.h"

#include "main.h"

/* Define for connection */

/* Declare Private Macro */

// #define		pin_low(port,pin);	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET);

__STATIC_INLINE void pin_low(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BRR, (PinMask >> GPIO_PIN_MASK_POS) & 0x0000FFFFU);
}

__STATIC_INLINE void pin_high(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, (PinMask >> GPIO_PIN_MASK_POS) & 0x0000FFFFU);
}

// #define		pin_high(port,pin);	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET);

/* Declare Function Prototype */
/* Declare Private Variable */
void tft_reset(void);
uint8_t tft_read_bus(void);
uint8_t tft_read_reg(uint8_t r);
void tft_write_bus(uint8_t byte);
void tft_write_color_fast(uint16_t data);
void tft_write_data(uint16_t data);
void tft_write_data16(uint16_t data);
void tft_write_reg(uint16_t data);
void tft_write_reg16(uint16_t data);
void tft_write_reg_data(uint16_t addr, uint16_t data);
void tft_write_reg16_data16(uint16_t addr, uint16_t data);

#endif // TFT_IO_HEADER_H
