#ifndef TFT_gfx_H
#define TFT_gfx_H

//-------------begin------------------------------
/* Include Library */
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "pins_ext.h"

#include "utoa.h"
#include "tft_defines.h"
#include "tft_base.h"
//-------------end--------------------------------

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

/* Declare Function Prototype */

/******************************** User Function ***********************************/

//=====================================================================================

uint16_t color565_to_555(uint16_t color);
uint16_t color555_to_565(uint16_t color);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void draw_pixel(int16_t x, int16_t y, uint16_t color);
void tft_draw_line(uint16_t column1, uint16_t row1, uint16_t column2, uint16_t row2, uint16_t color);
void tft_draw_dot_line(uint16_t column1, uint16_t row1, uint16_t column2, uint16_t row2, uint16_t color);
void pushColors1(uint16_t * block, int16_t n, bool first);
void pushColors2(uint8_t * block, int16_t n, bool first);
void pushColors3(const uint8_t * block, int16_t n, bool first, bool bigend);
void pushColors_any(uint16_t cmd, uint8_t * block, int16_t n, uint8_t first, uint8_t flags);
//----------------------------------
void fillScreen(uint16_t color);
void tft_clear(uint16_t color);
void tft_draw_point(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t size,uint16_t color);
void tft_draw_point_bold(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_slim(uint16_t x,uint16_t y,uint16_t color);
void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_fill_fast(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_fill_grid(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts8x16(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
//=====================================================================================
void write_string(uint16_t x, uint16_t y, int8_t* label);
void write_value(uint16_t x, uint16_t y, int16_t value);
void write_value_x(uint16_t x, uint16_t y, uint16_t value);
void write_value_hex(uint16_t x, uint16_t y, uint16_t value);

#endif // TFT_gfx_H
