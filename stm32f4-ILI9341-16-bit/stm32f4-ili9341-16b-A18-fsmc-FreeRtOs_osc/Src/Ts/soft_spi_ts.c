#include "stdint.h"
#include "soft_spi_ts.h"

uint8_t clock_polar;
uint8_t clock_phase;
    
uint32_t _delay;
uint8_t _order;

// #define GPIO_Read(port,pin) HAL_GPIO_ReadPin(port,pin)
// #define GPIO_Write(port,pin,state) HAL_GPIO_WritePin(port,pin,state)

void delay_tics(uint32_t tics)
{
  uint32_t count=0;
  for(count=0;count<tics;count++)
  {
    __asm volatile ("NOP");
  }
}

uint8_t reverse8(uint8_t x)
{
  x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1));
  x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2));
  return ((x >> 4) | (x << 4));
}

uint16_t reverse12(uint16_t val)
{
  uint16_t v2 =
  // (val & 0x01) << 15) |
  // (val & 0x02) << 13) |
  ((val & 0x04) << 11) |
  ((val & 0x08) << 9) |
  ((val & 0x10) << 7) |
  ((val & 0x20) << 5) |
  ((val & 0x40) << 3) |
  ((val & 0x80) << 1) |
  ((val & 0x100) >> 1) |
  ((val & 0x200) >> 3) |
  ((val & 0x400) >> 5) |
  ((val & 0x800) >> 7) |
  ((val & 0x1000) >> 9) |
  ((val & 0x2000) >> 11);// |
  // (val & 0x4000) >> 13)|
  // (val & 0x8000) >> 15);
  val = v2;
  return (val);
}

uint16_t reverse16(uint16_t x)
{
  x = (((x & 0xaaaa) >> 1) | ((x & 0x5555) << 1));
  x = (((x & 0xcccc) >> 2) | ((x & 0x3333) << 2));
  x = (((x & 0xf0f0) >> 4) | ((x & 0x0f0f) << 4));
  return ((x >> 8) | (x << 8));
}

uint32_t reverse32(uint32_t x)
{
  x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
  x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
  return ((x >> 16) | (x << 16));
}

void soft_spi_init(void)
{
  _delay = 2;
  clock_phase = 0;
  clock_polar = 0;
}

void soft_spi_begin(void)
{
  GPIO_Write(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, 0);
  /*
  pin_mode(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, GPIO_MODE_OUTPUT_PP);
  pin_mode(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin, GPIO_MODE_INPUT);
  pin_mode(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, GPIO_MODE_OUTPUT_PP);
  pin_mode(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_MODE_OUTPUT_PP);
  */
}

void soft_spi_end(void)
{
  GPIO_Write(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, 1);
  // pin_mode(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, GPIO_MODE_INPUT);
  // pin_mode(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin, GPIO_MODE_INPUT);
  // pin_mode(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, GPIO_MODE_INPUT);
  // pin_mode(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_MODE_INPUT);

  // HAL_GPIO_DeInit(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin);
  // HAL_GPIO_DeInit(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
  // HAL_GPIO_DeInit(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin);
  // HAL_GPIO_DeInit(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);
}

void soft_spi_set_bit_order(uint8_t order)
{
  switch(order)
  {
   case SPI_LSB_FIRST:
    _order = SPI_LSB_FIRST;
    break;
   case SPI_MSB_FIRST:
    _order = SPI_MSB_FIRST;
    break;
  }
}

void soft_spi_set_data_mode(uint8_t mode)
{
  switch(mode)
  {
   case SPI_MODE0:
    clock_polar = 0;
    clock_phase = 0;
    break;
   case SPI_MODE1:
    clock_polar = 0;
    clock_phase = 1;
    break;
   case SPI_MODE2:
    clock_polar = 1;
    clock_phase = 0;
    break;
   case SPI_MODE3:
    clock_polar = 1;
    clock_phase = 1;
    break;
  }
}

void soft_spi_set_clock_divider(uint32_t div)
{
  _delay = div;
}

void soft_spi_clk(uint8_t size)
{
  uint32_t del = _delay >> 1;
  // soft_spi_begin();
  for(uint8_t index = size; index > 0; index--)
  {
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? HIGH : LOW);
    delay_tics(del);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? LOW : HIGH);
    delay_tics(del);
  }
  // soft_spi_end();
}

void soft_spi_write(uint16_t val)
{
  // soft_spi_begin();
  if(_order == SPI_LSB_FIRST)
  {
    reverse8(val);
  }
  
  for(uint16_t bit = 0; bit < 8; bit++)
  {
    GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, val & (1<<bit) ? HIGH : LOW);
    soft_spi_clk(1);
  }
  // soft_spi_end();
}

void soft_spi_write8(uint16_t val)
{
  // soft_spi_begin();
  if(_order == SPI_LSB_FIRST)
  {
    reverse8(val);
  }

  for(uint16_t bit = 0; bit < 8; bit++)
  {
    GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, val & (1<<bit) ? HIGH : LOW);
    soft_spi_clk(1);
  }
  // soft_spi_end();
}

void soft_spi_write16(uint16_t val)
{
  // soft_spi_begin();
  if(_order == SPI_LSB_FIRST)
  {
    reverse16(val);
  }

  for(uint16_t bit = 0; bit < 16; bit++)
  {
    GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, val & (1<<bit) ? HIGH : LOW);
    soft_spi_clk(1);
  }
  // soft_spi_end();
}

uint16_t soft_spi_read(uint8_t size)
{
  // soft_spi_begin();
  int Data;
  Data = 0;
  // uint32_t del = _delay >> 1;

  for(uint8_t i = size; i > 0; i--)
  {
    Data += GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin) << (i-1);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? HIGH : LOW);
    // delay_tics(del);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? LOW : HIGH);
    // delay_tics(del);
  }

  if(_order == SPI_LSB_FIRST)
  {
    reverse16(Data);
  }

  return Data;
  // soft_spi_end();
}

uint8_t soft_spi_transfer8(uint8_t value)
{
  uint8_t Read_data = 0;

  if(_order == SPI_LSB_FIRST)
  {
    reverse8(value);
  }

  uint32_t del = _delay >> 1;
  delay_tics(del);

  uint8_t Read_miso = 0;

  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);

  for(uint8_t bit = 0; bit < 8; bit++) 
  {
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);
    // delay_tics(del);
    if(clock_phase) 
    {
      Read_miso = GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST) 
      {
        Read_data <<= 1;
        Read_data |= Read_miso;
      }
      else 
      {
        Read_data >>= 1;
        Read_data |= Read_miso << 7;
      }
    }
    else 
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((value<<bit) & 0x80) ? HIGH : LOW);
    }

    // delay_tics(del);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 1);
    // delay_tics(del);

    if(clock_phase) 
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((value<<bit) & 0x80) ? HIGH : LOW);
    }
    else 
    {
      Read_miso = GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST) 
      {
        Read_data <<= 1;
        Read_data |= Read_miso;
      }
      else 
      {
        Read_data >>= 1;
        Read_data |= Read_miso << 7;
      }
    }
    // delay_tics(del);
  }
  return Read_data;
}

uint16_t soft_spi_transfer16(uint16_t value)
{
  uint8_t Read_data = 0;

  if(_order == SPI_LSB_FIRST)
  {
    reverse16(value);
  }

  uint32_t del = _delay >> 1;
  delay_tics(del);

  uint8_t Read_miso = 0;

  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);

  for(uint8_t bit = 0; bit < 16; bit++) 
  {
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);
    // delay_tics(del);
    if(clock_phase) 
    {
      Read_miso = GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST) 
      {
        Read_data <<= 1;
        Read_data |= Read_miso;
      }
      else 
      {
        Read_data >>= 1;
        Read_data |= Read_miso << 15;
      }
    }
    else 
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((value<<bit) & 0x8000) ? HIGH : LOW);
    }

    // delay_tics(del);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 1);
    // delay_tics(del);

    if(clock_phase) 
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((value<<bit) & 0x8000) ? HIGH : LOW);
    }
    else 
    {
      Read_miso = GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST) 
      {
        Read_data <<= 1;
        Read_data |= Read_miso;
      }
      else 
      {
        Read_data >>= 1;
        Read_data |= Read_miso << 15;
      }
    }
    // delay_tics(del);
  }
  return Read_data;
}
