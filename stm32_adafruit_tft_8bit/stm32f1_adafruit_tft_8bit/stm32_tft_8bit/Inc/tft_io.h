#ifndef __tft_io_H
#define __tft_io_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"

#define GPIOA_Hi    (*(volatile unsigned long*)(GPIOA_BASE + 0x10))
#define GPIOA_Lo    (*(volatile unsigned long*)(GPIOA_BASE + 0x14))

#define GPIOB_Hi    (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_Lo    (*(volatile unsigned long*)(GPIOB_BASE + 0x14))

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET)

/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_RS_LOW()      HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
#define LCD_RS_HIGH()     HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)

#define LCD_RST_LOW()      HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_RST_HIGH()     HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#define LCD_WR_LOW()      HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)
#define LCD_WR_HIGH()     HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)

#define LCD_RD_LOW()      HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_RESET)
#define LCD_RD_HIGH()     HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)

void LCD_IO_Init(void);
void LCD_Delay(uint32_t cycle_count);
void tft_write_strob(void);
void tft_write_data(uint16_t data);
void tft_write_data16(uint16_t data);
void tft_write_reg(uint8_t data);
void tft_write_reg16(uint8_t data);
void LCD_IO_WriteBus(uint8_t byte);
void LCD_IO_WriteData(uint16_t data);
void LCD_IO_WriteReg(uint8_t data);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void WritePixel(uint16_t x,uint16_t y,uint16_t color);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __tft_io_H */

/***********************************END OF FILE***********************************/
