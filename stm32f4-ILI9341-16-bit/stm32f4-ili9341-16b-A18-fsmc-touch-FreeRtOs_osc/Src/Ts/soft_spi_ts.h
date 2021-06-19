#ifndef _soft_spi_ts_H
#define _soft_spi_ts_H

#include "main.h"

#define LOW 0
#define HIGH 1

#define SPI_MSB_FIRST 12
#define SPI_LSB_FIRST 13

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128
#define SPI_CLOCK_DIV256 256

#define GPIO_Read(port,pin) HAL_GPIO_ReadPin(port,pin)
#define GPIO_Write(port,pin,state) HAL_GPIO_WritePin(port,pin,state)

void delay_tics(uint32_t tics);
uint8_t reverse8(uint8_t x);
uint16_t reverse12(uint16_t val);
uint16_t reverse16(uint16_t x);
uint32_t reverse32(uint32_t x);
void soft_spi_init(void);
void soft_spi_begin(void);
void soft_spi_end(void);
void soft_spi_set_bit_order(uint8_t order);
void soft_spi_set_data_mode(uint8_t mode);
void soft_spi_set_clock_divider(uint32_t div);
void soft_spi_clk(uint8_t size);
void soft_spi_write(uint16_t val);
void soft_spi_write8(uint16_t val);
void soft_spi_write16(uint16_t val);
uint16_t soft_spi_read(uint8_t size);
uint8_t soft_spi_transfer8(uint8_t value);
uint16_t soft_spi_transfer16(uint16_t value);

#endif // _soft_spi_ts_H

