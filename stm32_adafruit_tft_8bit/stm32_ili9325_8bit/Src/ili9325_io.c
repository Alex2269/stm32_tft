#include "ili9325_io.h"

#define LCD_PORT GPIOA

extern uint16_t lcd_width;
extern uint16_t lcd_height;
uint16_t _rotation;

static inline
void delay_us(uint32_t delay_us)
{
  volatile uint32_t Delay = delay_us * (SystemCoreClock / 8U / 1000000U);
  while (Delay --) __asm__ volatile ("nop");
}

static inline
void delay(uint32_t delay_ms)
{
  volatile uint32_t Delay = delay_ms * (SystemCoreClock / 8U / 1000U);
  while (Delay --) __asm__ volatile ("nop");
}

uint16_t width(void)
{
  return lcd_width;
}

uint16_t height(void)
{
  return lcd_height;
}

void LCD_IO_Init(void)
{
  pin_low(LCD_RST_GPIO_Port,LCD_RST_Pin);
  delay(20);
  pin_high(LCD_RST_GPIO_Port,LCD_RST_Pin);
  delay(20);
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
  LCD_PORT->ODR = ((byte >> 2) | (byte << 6));
}

void tft_write_strob(void)
{
  pin_low(LCD_WR_GPIO_Port,LCD_WR_Pin);
  pin_high(LCD_WR_GPIO_Port,LCD_WR_Pin);
}

void tft_write_data(uint16_t data)
{
  pin_high(LCD_RS_GPIO_Port,LCD_RS_Pin);
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void tft_write_data16(uint16_t data)
{
  pin_high(LCD_RS_GPIO_Port,LCD_RS_Pin);
  LCD_IO_WriteBus((uint8_t)data>>8);
  tft_write_strob();
  LCD_IO_WriteBus((uint8_t)data&0x00ff);
  tft_write_strob();
}

void tft_write_reg(uint8_t reg)
{
  pin_low(LCD_RS_GPIO_Port,LCD_RS_Pin);
  LCD_IO_WriteBus((uint8_t)reg&0x00ff);
  tft_write_strob();
}

void tft_write_reg16(uint16_t reg)
{
  pin_low(LCD_RS_GPIO_Port,LCD_RS_Pin);
  LCD_IO_WriteBus((uint8_t)reg>>8);
  tft_write_strob();
  LCD_IO_WriteBus((uint8_t)reg&0x00ff);
  tft_write_strob();
}

void LCD_IO_WriteData(uint16_t data)
{
  tft_write_data(data);
}

void LCD_IO_WriteReg(uint8_t data)
{
  tft_write_reg(data);
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) 
{
  uint32_t readvalue = 0;

  pin_low(LCD_CS_GPIO_Port,LCD_CS_Pin);  /* Select: Chip Select low */
  pin_low(LCD_RS_GPIO_Port,LCD_RS_Pin);  /* Reset WRX_DCX to send command */

  // readvalue = SPI_Read(LCD_PORT);

  pin_high(LCD_RS_GPIO_Port,LCD_RS_Pin);  /* Set WRX_DCX to send data */
  pin_high(LCD_CS_GPIO_Port,LCD_CS_Pin);  /* Deselect: Chip Select high */

  return readvalue;
}

void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_write_reg16(0x0020); tft_write_data16(x1); tft_write_data16(x2);
  tft_write_reg16(0x0021); tft_write_data16(y1); tft_write_data16(y2);
  tft_write_reg16(0x0022);
}

// void WritePixel(uint16_t x,uint16_t y,uint16_t color)
// {
//   tft_address_set(x,y,x,y);
//   LCD_IO_WriteData(color>>8);
//   LCD_IO_WriteData(color&0x00ff);
// }

void WritePixel(uint16_t x,uint16_t y,uint16_t color)
{
#define rotation 0
  int16_t t;
  switch(rotation) 
  {
    case 1:
    t = x;
    x = lcd_width - 1 - y;
    y = t;
    break;
    case 2:
    x = lcd_width - 1 - x;
    y = lcd_height - 1 - y;
    break;
    case 3:
    t = x;
    x = y;
    y = lcd_height - 1 - t;
    break;
  }
  tft_address_set(x,y,x,y);
  LCD_IO_WriteData(color>>8);
  LCD_IO_WriteData(color&0x00ff);
}

// void WritePixel(uint16_t x, uint16_t y, uint16_t color)
// {
//   tft_write_reg16(0x0020); tft_write_data16(x); // Set GRAM horizontal Address
//   tft_write_reg16(0x0021); tft_write_data16(y); // Set GRAM Vertical Address
//   tft_write_reg16(0x0022); tft_write_data16(color); // Write Data to GRAM
// }

// static uint32_t SPI_Read(GPIO_TypeDef* GPIOx)
// {
//   return ((uint16_t)GPIOx->IDR);
// }
