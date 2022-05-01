#ifndef __ili9325_io_H
#define __ili9325_io_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
// #include "system_stm32f1xx.h"
// #include "stm32f1xx_ll_rcc.h"
// #include "stm32f1xx_ll_utils.h"
// #include "stm32f1xx_ll_pwr.h"
#include "gpio.h"

#define GPIOA_Hi    (*(volatile unsigned long*)(GPIOA_BASE + 0x10))
#define GPIOA_Lo    (*(volatile unsigned long*)(GPIOA_BASE + 0x14))

#define GPIOB_Hi    (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_Lo    (*(volatile unsigned long*)(GPIOB_BASE + 0x14))

/**
  * @brief  SD Control Lines management
  */

#define		pin_low(port,pin)  LL_GPIO_ResetOutputPin(port, pin)
#define		pin_high(port,pin)  LL_GPIO_SetOutputPin(port, pin)

/**
  * @brief  LCD Control Lines management
  */

void delay_us(uint32_t delay_us);
void delay(uint32_t delay_ms);
uint16_t width(void);
uint16_t height(void);
void LCD_IO_Init(void);
void LCD_Delay(uint32_t cycle_count);
void tft_write_strob(void);
void tft_write_data(uint16_t data);
void tft_write_data16(uint16_t data);
void tft_write_reg(uint8_t reg);
void tft_write_reg16(uint16_t reg);
void LCD_IO_WriteBus(uint8_t byte);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void WritePixel(uint16_t x,uint16_t y,uint16_t color);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __ili9325_io_H */

/***********************************END OF FILE***********************************/
