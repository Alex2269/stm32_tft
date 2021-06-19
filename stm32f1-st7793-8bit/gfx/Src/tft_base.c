#include "tft_init.h"
#include "tft_base.h"


uint16_t tft_W = 0;
uint16_t tft_H = 0;
uint16_t BACK_COLOR = 0;
uint16_t POINT_COLOR = 0;
uint16_t _MC, _MP, _MW, _SC, _EC, _SP, _EP;
static uint16_t _width, _height;
uint16_t r = 1;

uint16_t set_width(uint16_t width)
{
  _width = width;
  return _width;
}

uint16_t set_height(uint16_t height)
{
  _height = height;
  return _height;
}

uint16_t get_width(void)
{
  return _width;
}

uint16_t get_height(void)
{
  return _height;
}

void reset(void)
{
  CS_IDLE;
  RD_IDLE;
  WR_IDLE;
  RESET_IDLE;
  delay(50);
  RESET_ACTIVE;
  delay(100);
  RESET_IDLE;
  delay(100);
}

void tft_write_bus(uint8_t byte)
{
  GPIOA->ODR = (byte >> 2) | (byte << 6);
}

void tft_wr_pin_strob(void)
{
  GPIOB_Lo = 1<<WR;
  GPIOB_Hi = 1<<WR;
}

void tft_wr_pin_strob2(void)
{
  GPIOB_Lo = 1<<WR;
  GPIOB_Hi = 1<<WR;
  __asm volatile ("nop");
  __asm volatile ("nop");
  __asm volatile ("nop");
  __asm volatile ("nop");
  GPIOB_Lo = 1<<WR;
  GPIOB_Hi = 1<<WR;
}

void tft_write_reg16(uint16_t data)
{
  GPIOB_Lo = 1<<CS;
  GPIOB_Lo = 1<<RS;
  tft_write_bus(data>>8);
  tft_wr_pin_strob();
  tft_write_bus(data&0x00ff);
  tft_wr_pin_strob();
  GPIOB_Hi = 1<<RS;
  GPIOB_Hi = 1<<CS;
}

void tft_write_color_fast(uint16_t data)
{
  GPIOB_Lo = 1<<CS;
  tft_write_bus(data>>8);
  GPIOB_Lo = 1<<WR;
  GPIOB_Hi = 1<<WR;
  tft_write_bus(data&0x00ff);
  GPIOB_Lo = 1<<WR;
  GPIOB_Hi = 1<<WR;
  GPIOB_Hi = 1<<CS;
}

void tft_write_data16(uint16_t data)
{
  GPIOB_Lo = 1<<CS;
  tft_write_bus(data>>8);
  tft_wr_pin_strob();
  tft_write_bus(data&0x00ff);
  tft_wr_pin_strob();
  GPIOB_Hi = 1<<CS;
}

void WriteCmdData(uint16_t cmd, uint16_t data)
{
  GPIOB_Lo = 1<<CS;
  GPIOB_Lo = 1<<RS;
  tft_write_bus(cmd>>8);
  tft_wr_pin_strob();
  tft_write_bus(cmd&0x00ff);
  tft_wr_pin_strob();
  GPIOB_Hi = 1<<RS;
  tft_write_bus(data>>8);
  tft_wr_pin_strob();
  tft_write_bus(data&0x00ff);
  tft_wr_pin_strob();
  GPIOB_Hi = 1<<CS;
}

void write_command_data(uint16_t cmd, uint16_t data)
{
  tft_write_reg16(cmd);
  tft_write_data16(data);
}

void setRotation(uint8_t r)
{
  if(r==0||r==1) WriteCmdData(0x400, 0x6a00);
  if(r==2||r==3) WriteCmdData(0x400, 0xea00);
  if(r==1||r==2) WriteCmdData(0x01, 0x0000);
  if(r==0||r==3) WriteCmdData(0x01, 0x0100);
  if(r==0||r==2) WriteCmdData(0x03, 0x1030);
  if(r==1||r==3) WriteCmdData(0x03, 0x1038);
  
  if(r==(r&0x02))
  {
    tft_H = get_width();
    tft_W = get_height();
    _MC = 0x0200; _MP = 0x0201; _MW = 0x0202; _EC = 0x0211; _EP = 0x0213; _SC = 0x0210; _SP = 0x0212;
  }
  
  if(r==(r&0x01))
  {
    tft_W = get_width();
    tft_H = get_height();
    _MC = 0x0201; _MP = 0x0200; _MW = 0x0202; _EC = 0x0213; _EP = 0x0211; _SC = 0x0212; _SP = 0x0210;
  }
  setAddrWindow(0, 0, tft_W - 1, tft_H - 1);
}

void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1)
{
  WriteCmdData(_MC, x);
  WriteCmdData(_MP, y);
  if (!(x == x1 && y == y1))
  {
    WriteCmdData(_SC, x);
    WriteCmdData(_SP, y);
    WriteCmdData(_EC, x1);
    WriteCmdData(_EP, y1);
  }
  tft_write_reg16(0x0202);
}

void init_table16(const void *table, int16_t size)
{
  uint16_t *p = (uint16_t *) table;
  while (size > 0)
  {
    uint16_t dat;
    uint16_t cmd = *p++;
    uint16_t len = *p++;
    if (cmd == TFTLCD_DELAY)
    {
      delay(len);
      len = 0;
    }
    else
    {
      tft_write_reg16(cmd);
      for (uint16_t i = 0; i < len; i++)
      if(len)
      {
        dat = *p++;
        tft_write_data16(dat);
      }
    }
    size -= len + 2 * sizeof(int16_t);
  }
}

void init_tft(void)
{
  reset();
  pin_low(CS_PORT,CS_PIN);
  pin_low(RST_PORT,RST_PIN);
  HAL_Delay(120);
  pin_high(RST_PORT,RST_PIN);
  HAL_Delay(120);
  pin_low(CS_PORT,CS_PIN);
  init_table16(&ST7793_regValues, sizeof(ST7793_regValues));
  setRotation(0);
}

void tft_address_set(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  setAddrWindow(x, y, w, h);
}
