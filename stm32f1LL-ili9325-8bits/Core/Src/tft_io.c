#include "main.h"
#include "tft_io.h"

/* Declare Private Variable */
static inline
void delay_us(uint32_t volatile microsec)
{
  microsec *=(SystemCoreClock/1000000)/5;
  while(microsec--);
}

static inline
void delay(uint32_t volatile msec)
{
  msec *=(SystemCoreClock/1000)/5;
  while(msec--);
}

void tft_reset(void)
{
  pin_high(DSP_LED_GPIO_Port,DSP_LED_Pin);
  pin_high(DSP_CS_GPIO_Port,DSP_CS_Pin);
  pin_high(DSP_RD_GPIO_Port,DSP_RD_Pin);
  pin_high(DSP_WR_GPIO_Port,DSP_WR_Pin);
  pin_low(DSP_RST_GPIO_Port,DSP_RST_Pin);
  delay(120);
  pin_high(DSP_RST_GPIO_Port,DSP_RST_Pin);
  delay(120);
  pin_low(DSP_CS_GPIO_Port,DSP_CS_Pin);
}

uint8_t tft_read_bus(void)
{
  uint8_t data;
  data = (uint8_t)GPIOA->IDR;
  // return data;
  return ((data << 2) | (data >> 6));
}

uint8_t tft_read_reg(uint8_t r)
{
  uint32_t id;
  tft_write_reg(r);
  pin_low(DSP_RS_GPIO_Port,DSP_RS_Pin);
  pin_low(DSP_RD_GPIO_Port,DSP_RD_Pin);
  pin_high(DSP_RD_GPIO_Port,DSP_RD_Pin);
  id = tft_read_bus();
  return id;
}

void tft_write_bus(uint8_t byte)
{
  // ! GPIOA A0 A1 A2 A3 A4 A5 A6 A7
  // ! TFT D0 D1 D2 D3 D4 D5 D6 D7
  // ! GPIOA->ODR = byte;
  // ! circular shift right of byte on 2 bits,
  // ! to save from the cross direction of wires, on tft shield
  // ! GPIOA A0 A1 A2 A3 A4 A5 A6 A7 (byte >> 2) | (byte << 6)
  // ! _TFT_ D2 D3 D4 D5 D6 D7 D0 D1
  GPIOA->ODR = (byte >> 2) | (byte << 6);
  // ! uint8_t shift;
  // ! shift=(byte >> 2) | (byte << 6);
  // ! for(uint8_t i=0;i<8;i++)
  // ! (shift&(0x01<<i))?(GPIOA_Hi = 1<<i):(GPIOA_Lo = 1<<i);
  // ! (shift&(0x01<<i))?(pin_high(PORT_NAME_ARR[i],PIN_NAME_ARR[i])):(pin_low(PORT_NAME_ARR[i],PIN_NAME_ARR[i]));
}

void tft_write_color_fast(uint16_t data)
{
  pin_high(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_write_bus(data>>8);
  pin_low(DSP_WR_GPIO_Port,DSP_WR_Pin);
  pin_high(DSP_WR_GPIO_Port,DSP_WR_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(DSP_WR_GPIO_Port,DSP_WR_Pin);
  pin_high(DSP_WR_GPIO_Port,DSP_WR_Pin);
}

void tft_write_data(uint16_t data)
{
  pin_high(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(DSP_WR_GPIO_Port,DSP_WR_Pin);
  pin_high(DSP_WR_GPIO_Port,DSP_WR_Pin);
}

void tft_write_data16(uint16_t data)
{
  tft_write_data(data >> 8);
  tft_write_data(data&0x00ff);
}

void tft_write_reg(uint16_t data)
{
  pin_low(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(DSP_WR_GPIO_Port,DSP_WR_Pin);
  pin_high(DSP_WR_GPIO_Port,DSP_WR_Pin);
}

void tft_write_reg16(uint16_t data)
{
  tft_write_reg(data >> 8);
  tft_write_reg(data&0x00ff);
}

void tft_write_reg_data(uint16_t addr, uint16_t data)
{
  tft_write_reg(addr);
  tft_write_data(data);
}

void tft_write_reg16_data16(uint16_t addr, uint16_t data)
{
  tft_write_reg16(addr);
  tft_write_data16(data);
}

