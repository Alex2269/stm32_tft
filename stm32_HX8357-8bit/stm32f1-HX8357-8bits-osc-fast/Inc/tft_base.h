#ifndef TFT_BASE_HEADER_H
#define TFT_BASE_HEADER_H

/******************** TFT-480-QVT-9481x tft LIBRARY ***********************
******** Description : This is library for TFT-9481x using STM32F103C8T6 uC 
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "main.h"

/* Define for connection */

#define 	RS_PORT		DSP_RS_GPIO_Port
#define		RS_PIN		DSP_RS_Pin

#define		WR_PORT		DSP_WR_GPIO_Port
#define 	WR_PIN		DSP_WR_Pin

#define		RD_PORT		DSP_RD_GPIO_Port
#define		RD_PIN		DSP_RD_Pin

#define		CS_PORT		DSP_CS_GPIO_Port
#define		CS_PIN		DSP_CS_Pin

#define		REST_PORT	DSP_RST_GPIO_Port
#define		REST_PIN	DSP_RST_Pin

#define		RST_PORT	DSP_RST_GPIO_Port
#define		RST_PIN		DSP_RST_Pin

#define		D0_PORT		DSP_0_GPIO_Port
#define		D0_PIN		DSP_0_Pin

#define		D1_PORT		DSP_1_GPIO_Port
#define		D1_PIN		DSP_1_Pin

#define		D2_PORT		DSP_2_GPIO_Port
#define		D2_PIN		DSP_2_Pin

#define		D3_PORT		DSP_3_GPIO_Port
#define		D3_PIN		DSP_3_Pin

#define		D4_PORT		DSP_4_GPIO_Port
#define		D4_PIN		DSP_4_Pin

#define		D5_PORT		DSP_5_GPIO_Port
#define		D5_PIN		DSP_5_Pin

#define		D6_PORT		DSP_6_GPIO_Port
#define		D6_PIN		DSP_6_Pin

#define		D7_PORT		DSP_7_GPIO_Port
#define		D7_PIN		DSP_7_Pin

#define		D8_PORT		DSP_8_GPIO_Port
#define		D8_PIN		DSP_8_Pin

#define		D9_PORT		DSP_9_GPIO_Port
#define		D9_PIN		DSP_9_Pin

#define		D10_PORT	DSP_10_GPIO_Port
#define		D10_PIN		DSP_10_Pin

#define		D11_PORT	DSP_11_GPIO_Port
#define		D11_PIN		DSP_11_Pin

#define		D12_PORT	DSP_12_GPIO_Port
#define		D12_PIN		DSP_12_Pin

#define		D13_PORT	DSP_13_GPIO_Port
#define		D13_PIN		DSP_13_Pin

#define		D14_PORT	DSP_14_GPIO_Port
#define		D14_PIN		DSP_14_Pin

#define		D15_PORT	DSP_15_GPIO_Port
#define		D15_PIN		DSP_15_Pin

/* Declare Private Macro */
#define		pin_low(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET)
#define		pin_high(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET)

#define tft_write_reg_data(reg, data)	{ tft_write_reg(reg); tft_write_data(data); }

#define		max(a,b)		(a>b)?a:b
#define		min(a,b)		(a<b)?a:b
#define		abs(a)			(a>=0)?a:(-a)

#define RD 5
#define WR 6
#define RS 7
#define CS 8
#define RST 9

#define GPIOA_Hi    (*(volatile unsigned long*)(GPIOA_BASE + 0x10))
#define GPIOA_Lo    (*(volatile unsigned long*)(GPIOA_BASE + 0x14))

#define GPIOB_Hi    (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_Lo    (*(volatile unsigned long*)(GPIOB_BASE + 0x14))

/* Declare TFT Dimension */

#define COLOR_WHITE			0xFFFF
#define COLOR_BLACK			0x0000
#define COLOR_BLUE			0x001F
#define COLOR_BRED             		0xF81F
#define COLOR_GRED			0xFFE0
#define COLOR_GBLUE			0x07FF
#define COLOR_RED           	 	0xF800
#define COLOR_VIOLET	       	 	0xF81F
#define COLOR_GREEN         	 	0x07E0
#define COLOR_CYAN          	 	0x7FFF
#define COLOR_YELLOW        	 	0xFFE0
#define COLOR_BROWN			0xBC40
#define COLOR_BRRED			0xFC07
#define COLOR_GRAY			0x8430
#define COLOR_DARKBLUE      	 	0x01CF
#define COLOR_LIGHTBLUE      	 	0x7D7C
#define COLOR_GRAYBLUE       	 	0x5458
#define COLOR_LIGHTGREEN     	 	0x841F
#define COLOR_LGRAY			0xC618
#define COLOR_LGRAYBLUE			0xA651
#define COLOR_LBBLUE			0x2B12

/* Define TFT String Mode */
#define TFT_STRING_MODE_NO_BACKGROUND	0x01
#define TFT_STRING_MODE_BACKGROUND	0x00

uint16_t tft_W;
uint16_t tft_H;

/* Declare Function Prototype */

/******************************** User Function ***********************************/
void init_table8(const void *table, int16_t size);
void init_table16(const void *table, int16_t size);
void tft_init(void);
void tft_SetRotation(uint8_t r);
void tft_clear(uint16_t color);
void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color);
void tft_draw_dot_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color);
void tft_draw_line_slim(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color);
void tft_draw_line_bold(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color);
void tft_draw_rectangle(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color);
void tft_draw_circle(uint16_t row,uint16_t column,uint16_t radian,uint16_t color);
void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_puts_image_pos(const unsigned char* image_arr, uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y);
void tft_puts_image(const unsigned char* image_arr);
void tft_puts8x16(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);

/******************************** Private Function *********************************/
// Private Function do not use in user program

void tft_write_bus(uint8_t byte);
void tft_wr_pin_strob(void);
void tft_wr_strob2(void);
void tft_write_color(uint8_t byte);
void tft_write_data(uint16_t data);
void tft_write_reg(uint16_t data);
void tft_address_set(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2);
void tft_draw_point(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_big2(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_bold(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_slim(uint16_t x,uint16_t y,uint16_t color);
void tft_write_data8(uint8_t high_byte,uint8_t low_byte);
void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);

#endif // TFT_BASE_HEADER_H
