#include "stdint.h"
#include "soft_spi_ts.h"

uint8_t clock_polar;
uint8_t clock_phase;
uint32_t _delay;
uint8_t _order;

// #define ReadInputPin(port,pin) HAL_GPIO_ReadPin(port,pin)
// #define WriteOutputPin(port,pin,state) HAL_GPIO_WritePin(port,pin,state)

void delay_tics(uint32_t tics)
{
  uint32_t count=0;
  for(count=0;count<tics;count++)
  {
    __asm volatile ("nop");
  }
}

uint32_t reverse_bits(uint32_t value, uint32_t len)
{
  len -= 1;
  uint32_t reverse_val = value;

  value >>= 1;
  while(value)
  {
    reverse_val <<= 1;
    reverse_val |= value & 1;
    value >>= 1;
    len--;
  }
  reverse_val <<= len;
  return reverse_val;
}

void soft_spi_init(void)
{
  _delay = 4;
  clock_phase = 0;
  clock_polar = 0;
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

void soft_spi_clk(uint8_t count)
{
  uint32_t del = _delay >> 1;
  // soft_spi_begin();
  for(uint8_t index = count; index > 0; index--)
  {
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? HIGH : LOW);
    delay_tics(del);
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? LOW : HIGH);
    delay_tics(del);
  }
  // soft_spi_end();
}

void soft_spi_write(uint16_t val, uint16_t len)
{
  uint32_t data;
  data = val;
  if(_order == SPI_MSB_FIRST)
  {
    data = reverse_bits(val, len);
  }

  for(uint16_t bit = 0; bit < len; bit++)
  {
    WriteOutputPin(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, data & (1<<bit) ? HIGH : LOW);
    soft_spi_clk(1);
  }
}

uint16_t soft_spi_read(uint16_t len)
{
  // soft_spi_begin();
  int data;
  data = 0;
  // uint32_t del = _delay >> 1;

  for(uint16_t i = len; i > 0; i--)
  {
    data += ReadInputPin(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin) << (i-1);
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? HIGH : LOW);
    // delay_tics(del);
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, clock_polar ? LOW : HIGH);
    // delay_tics(del);
  }

  if(_order == SPI_LSB_FIRST)
  {
    reverse_bits(data,16);
  }

  return data;
  // soft_spi_end();
}

uint16_t soft_spi_transfer(uint16_t data, uint16_t len)
{
  uint8_t read_data = 0;

  if(_order == SPI_LSB_FIRST)
  {
    reverse_bits(data, len);
  }

  uint32_t del = _delay >> 1;
  delay_tics(del);

  uint8_t read_miso = 0;

  WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);

  for(uint8_t bit = 0; bit < len; bit++)
  {
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 0);
    // delay_tics(del);
    if(clock_phase)
    {
      read_miso = ReadInputPin(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST)
      {
        read_data <<= 1;
        read_data |= read_miso;
      }
      else
      {
        read_data >>= 1;
        read_data |= read_miso << (len-1);
      }
    }
    else
    {
      WriteOutputPin(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((data<<bit) & (1 << (len-1))) ? HIGH : LOW);
    }

    // delay_tics(del);
    WriteOutputPin(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin, 1);
    // delay_tics(del);

    if(clock_phase)
    {
      WriteOutputPin(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, ((data<<bit) & (1 << (len-1))) ? HIGH : LOW);
    }
    else
    {
      read_miso = ReadInputPin(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin);
      if(_order == SPI_LSB_FIRST)
      {
        read_data <<= 1;
        read_data |= read_miso;
      }
      else
      {
        read_data >>= 1;
        read_data |= read_miso << (len-1);
      }
    }
    // delay_tics(del);
  }
  return read_data;
}

