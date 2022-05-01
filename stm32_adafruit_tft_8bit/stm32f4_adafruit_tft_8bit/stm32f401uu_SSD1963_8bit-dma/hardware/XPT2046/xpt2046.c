#include "xpt2046.h"
#include "soft_spi_ts.h"

#define CMD_X 0x99 // inquiry of coordinate X
#define CMD_Y 0x59 // inquiry of coordinate Y
#define CMD_Z 0x39 // inquiry Z information on pressing

TS_Point p;

void pin_pen_on(void)
{
  // pin_mode(T_IRQ_GPIO_Port, T_IRQ_Pin, GPIO_MODE_INPUT);
}

void touch_spi_init(void)
{
  soft_spi_set_bit_order(SPI_LSB_FIRST);
  soft_spi_set_data_mode(SPI_MODE0);
  soft_spi_set_clock_divider(SPI_CLOCK_DIV64);
}

// get_point(&p); // get coordinates x y z from xpt2046

void get_point(TS_Point *p)
{
  touch_spi_init();

  soft_spi_begin(); // *(touch_cs)= 0;
  soft_spi_clk(32); // flushing
  soft_spi_write(CMD_X,8);
  p->x = (1024-soft_spi_read(12))*0.355;  // prepare x position (0-320)
  soft_spi_end(); //   *(touch_cs)= 1;
  
  soft_spi_begin(); // *(touch_cs)= 0;
  soft_spi_clk(32); // flushing
  soft_spi_write(CMD_Y,8);
  p->y = (soft_spi_read(14)-160)*1.60; // prepare y position (0-240)
  soft_spi_end(); //   *(touch_cs)= 1;

  soft_spi_begin(); // *(touch_cs)= 0;
  soft_spi_clk(32); // flushing
  soft_spi_write(CMD_Z,8);
  p->z = soft_spi_read(12);
  soft_spi_end(); //   *(touch_cs)= 1;


  // p->z = ReadInputPin(TOUCH_PEN_GPIO_Port, TOUCH_PEN_Pin);
}
