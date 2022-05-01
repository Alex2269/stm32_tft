#ifndef __xpt_2046_H
#define __xpt_2046_H

#include "main.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"

#define CMD_RDX 0xD0
#define CMD_RDY 0x90

typedef struct TS_Point
{
  uint8_t str_x[8];
  uint8_t str_y[8];
  uint8_t str_z[8];

  int16_t x, y, z;
} TS_Point;

void Touch_ini(void);
void spi_ts_write(uint8_t TxData);
uint16_t ADS_Read_AD(unsigned char CMD);
uint16_t ADS_Read_XY(uint8_t xy);
int get_touch(TS_Point *p);

#endif /* __xpt_2046_H */
