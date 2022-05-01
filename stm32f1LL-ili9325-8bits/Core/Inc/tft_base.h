#ifndef TFT_BASE_HEADER_H
#define TFT_BASE_HEADER_H

/****************************** tft LIBRARY ******************************
** Description : This is library for TFT-ili9325 using STM32F103C8T6 uC **
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stdbool.h"
#include "tft_io.h"
#include "main.h"

/* Define for connection */

#define max(a,b);   (a>b);?a:b
#define min(a,b);   (a<b);?a:b
#define abs(a);     (a>=0);?a:(-a);

/* Declare TFT Dimension */

#define COLOR_WHITE         0xFFFF
#define COLOR_BLACK         0x0000
#define COLOR_BLUE          0x001F
#define COLOR_BRED          0xF81F
#define COLOR_GRED          0xFFE0
#define COLOR_GBLUE         0x07FF
#define COLOR_RED           0xF800
#define COLOR_VIOLET        0xF81F
#define COLOR_GREEN         0x07E0
#define COLOR_CYAN          0x7FFF
#define COLOR_YELLOW        0xFFE0
#define COLOR_BROWN         0xBC40
#define COLOR_BRRED         0xFC07
#define COLOR_GRAY          0x8430
#define COLOR_DARKBLUE      0x01CF
#define COLOR_LIGHTBLUE     0x7D7C
#define COLOR_GRAYBLUE      0x5458
#define COLOR_LIGHTGREEN    0x841F
#define COLOR_LGRAY         0xC618
#define COLOR_LGRAYBLUE     0xA651
#define COLOR_LBBLUE        0x2B12

/* Define TFT String Mode */
#define TFT_STRING_MODE_NO_BACKGROUND   0x01
#define TFT_STRING_MODE_BACKGROUND      0x00

/* Declare Function Prototype */

uint16_t get_width(void);
uint16_t get_height(void);
void set_width(uint16_t width);
void set_height(uint16_t height);
void delay_us(uint32_t volatile microsec);
void delay(uint32_t volatile msec);
void init_table(const void *table, int16_t size);
void init_table8(const void *table, int16_t size);
void init_table16(const void *table, int16_t size);
void lcd_model(uint16_t model, uint16_t width, uint16_t height);
void invertDisplay(bool i);
void lcdSetup(void);
void tft_SetRotation(uint8_t r);
void vertScroll(uint16_t top, uint16_t scrollines, uint16_t offset);
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDisplayOff(void);
void lcdDisplayOn(void);
void lcdFillScreen(uint16_t color);
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius, uint16_t color);
void lcd_FillEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius, uint16_t color);
void lcd_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t color);
void lcd_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t color);
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
void lcdDrawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
void lcdDrawFillArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
uint16_t rgb565_conv(uint16_t r, uint16_t g, uint16_t b);
void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_draw_point(uint16_t x, uint16_t y, uint16_t color);
void tft_draw_point_big(uint16_t x, uint16_t y, uint16_t color);
void tft_draw_point_big2(uint16_t x, uint16_t y, uint16_t color);
void tft_clear(uint16_t color);
void tft_fill(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color);
void tft_fill_rect(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color);
void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color);
void tft_draw_dot_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color);
void tft_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void tft_draw_circle(uint16_t row, uint16_t column, uint16_t radian, uint16_t color);
void tft_puts_image_pos(const unsigned char* image_arr, uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y);
void tft_puts_image(const unsigned char* image_arr);
void tft_putchar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode);
void tft_putchar14x24(uint16_t x, uint16_t y, uint8_t data, uint8_t mode);
void tft_putchar18x32(uint16_t x, uint16_t y, uint8_t data, uint8_t mode);
void tft_putchar26x48(uint16_t x, uint16_t y, uint8_t data, uint8_t mode);
void tft_puts26x48(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE);
void tft_puts18x32(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE);
void tft_puts14x24(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE);
void tft_puts8x16(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE);

#endif // TFT_BASE_HEADER_H
