#include "ili9341_io.h"

void LCD_IO_Init(void)
{
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(20);
}

void LCD_Delay(uint32_t cycle_count)
{
  uint32_t count=0;
  for(count=0;count<cycle_count*100;count++)
  {
    __asm volatile ("nop");
  }
}

void LCD_IO_WriteBus(uint8_t byte)
{
  GPIOA->ODR = ((byte >> 2) | (byte << 6));
}

void tft_write_strob(void)
{
  LCD_WR_LOW(); LCD_WR_HIGH();
}

void tft_write_data(uint16_t data)
{
  LCD_RS_HIGH();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void tft_write_data16(uint16_t data)
{
  LCD_RS_HIGH();
  LCD_IO_WriteBus((uint8_t)data>>8);
  tft_write_strob();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void tft_write_reg(uint8_t data)
{
  LCD_RS_LOW();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void tft_write_reg16(uint8_t data)
{
  LCD_RS_LOW();
  LCD_IO_WriteBus((uint8_t)data>>8);
  tft_write_strob();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void LCD_IO_WriteData(uint16_t data)
{
  // GPIOB_Hi = 1<<LCD_RS;
  LCD_RS_HIGH();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  // GPIOB_Lo = 1<<LCD_WR;
  LCD_WR_LOW();
  // GPIOB_Hi = 1<<LCD_WR;
  LCD_WR_HIGH();
}

void LCD_IO_WriteReg(uint8_t data)
{
  // GPIOB_Lo = 1<<LCD_RS;
  LCD_RS_LOW();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  // GPIOB_Lo = 1<<LCD_WR;
  LCD_WR_LOW();
  // GPIOB_Hi = 1<<LCD_WR;
  LCD_WR_HIGH();
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) 
{
  uint32_t readvalue = 0;

  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, 0);  /* Select: Chip Select low */
  HAL_GPIO_WritePin(GPIOA, LCD_RS_Pin, 0);  /* Reset WRX_DCX to send command */

  // readvalue = SPI_Read(GPIOA);

  HAL_GPIO_WritePin(GPIOA, LCD_RS_Pin, 1);  /* Set WRX_DCX to send data */
  HAL_GPIO_WritePin(GPIOA, LCD_CS_Pin, 1);  /* Deselect: Chip Select high */

  return readvalue;
}

void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  LCD_IO_WriteReg(0x2a);
  LCD_IO_WriteData(x1>>8);
  LCD_IO_WriteData(x1&0x00ff);
  LCD_IO_WriteData(x2>>8);
  LCD_IO_WriteData(x2&0x00ff);
  LCD_IO_WriteReg(0x2b);
  LCD_IO_WriteData(y1>>8);
  LCD_IO_WriteData(y1&0x00ff);
  LCD_IO_WriteData(y2>>8);
  LCD_IO_WriteData(y2&0x00ff);
  LCD_IO_WriteReg(0x2c);
}

void WritePixel(uint16_t x,uint16_t y,uint16_t color)
{
  tft_address_set(x,y,x,y);
  LCD_IO_WriteData(color>>8);
  LCD_IO_WriteData(color&0x00ff);
}

// static uint32_t SPI_Read(GPIO_TypeDef* GPIOx)
// {
//   return ((uint16_t)GPIOx->IDR);
// }
