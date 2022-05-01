#ifndef __osc_H
#define __osc_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#include "adc.h"
#include "lcd.h"
#include "bmp.h"

#include "time.h"
// #include "touch_hall.h"
#include "xpt2046.h"
#include "soft_spi_ts.h"
#include "stm32_adafruit_lcd.h"

void write_info_top(uint32_t ms);
void write_info_down(uint32_t us);
void write_info_touch(void);
void oscil_init(void);
uint32_t ClearTest(void);
void oscil_run(void);
void fill_adc_buff(uint16_t i);
void fill_buff(uint16_t i);
void rotate_buff(uint16_t r);
void write_graph(uint16_t i);
void write_grid(void);
void tft_draw_dot_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t solid);


#endif // __osc_H

