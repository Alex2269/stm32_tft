#ifndef ILI9341_H_
#define ILI9341_H_
//------------------------------------------------
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include "lcd.h"
#include "fatfs.h"

//----------------- address FSMS bus - has to be a volatile -----------------
// PD11   ------> FSMC_A16
// PD13   ------> FSMC_A18
#define ADDR_CMD  *(volatile uint16_t*)(0x60000000) // clears A16
#define ADDR_DATA *(volatile uint16_t*)(0x60000000+(2<<16)) // sets FSMC_A16
// #define ADDR_DATA *(volatile uint16_t*)(0x60000000+(2<<18)) // sets FSMC_A18
//---------------------------------------------------------------------------

#define swap(a,b) {int16_t t=a;a=b;b=t;}

#define 	RST_PORT	DSP_RST_GPIO_Port
#define		RST_PIN		DSP_RST_Pin

#define		pin_low(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET)
#define		pin_high(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET)

//------------------------------------------------
#define convert24to16(x) (((x & 0x00F80000)>>8)|((x & 0x0000FC00)>>5)|((x & 0x000000F8)>>3))
//------------------------------------------------
/* Declare Private Macro */
#define		pin_low(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET)
#define		pin_high(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET)

#define		max(a,b)		(a>b)?a:b
#define		min(a,b)		(a<b)?a:b
#define		abs(a)			(a>=0)?a:(-a)

/* Declare TFT Dimension */

#define WHITE			0xFFFF
#define BLACK			0x0000
#define BLUE			0x001F
#define BRED			0XF81F
#define GRED			0XFFE0
#define GBLUE			0X07FF
#define RED			0xF800
#define VIOLET			0xF81F
#define GREEN			0x07E0
#define CYAN			0x7FFF
#define YELLOW			0xFFE0
#define BROWN			0XBC40
#define BRRED			0XFC07
#define GRAY			0X8430
#define DARKBLUE		0X01CF
#define LIGHTBLU		0X7D7C
#define GRAYBLUE		0X5458
#define LIGHTGREEN		0X841F
#define LGRAY			0XC618
#define LGRAYBLUE        	0XA651
#define LBBLUE			0X2B12

#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000
#define COLOR_BLUE		0x001F
#define COLOR_BRED		0XF81F
#define COLOR_GRED		0XFFE0
#define COLOR_GBLUE		0X07FF
#define COLOR_RED		0xF800
#define COLOR_VIOLET		0xF81F
#define COLOR_GREEN		0x07E0
#define COLOR_CYAN		0x7FFF
#define COLOR_YELLOW		0xFFE0
#define COLOR_BROWN		0XBC40
#define COLOR_BRRED		0XFC07
#define COLOR_GRAY		0X8430
#define COLOR_DARKBLUE		0X01CF
#define COLOR_LIGHTBLU		0X7D7C
#define COLOR_GRAYBLUE		0X5458
#define COLOR_LIGHTGREEN	0X841F
#define COLOR_LGRAY		0XC618
#define COLOR_LGRAYBLUE		0XA651
#define COLOR_LBBLUE		0X2B12

/* Define TFT String Mode */
#define TFT_STRING_MODE_NO_BACKGROUND	0x01
#define TFT_STRING_MODE_BACKGROUND	0x00

uint16_t tft_W;
uint16_t tft_H;

/* Declare Function Prototype */
void tft_Delay(volatile uint32_t delay);
void tft_write_data(volatile uint16_t data);
void tft_write_reg(volatile uint16_t data);
uint32_t tft_ReadReg(uint8_t r);
void tft_SetRotation(uint8_t r);
void tft_Flood(uint16_t color,uint32_t len);
void tft_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_FillScreen(uint16_t color);
void tft_FillRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_DrawPixel(int64_t x, int64_t y, uint16_t color);
void tft_DrawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_DrawCircle(uint16_t x0, uint16_t y0, int64_t r, uint16_t color);
void tft_FontsIni(void);
void tft_DrawBitmap(uint16_t x,uint16_t y, char *s);
uint16_t tft_RandColor(void);
void tft_reset(void);
void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_ini(void);
void tft_clear(uint16_t color);
void tft_draw_point(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t color);
void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);


#endif /* ILI9341_H_ */



