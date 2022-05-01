#ifndef TFT_BASE_HEADER_H
#define TFT_BASE_HEADER_H

/******************** TFT-480-QVT-9481x tft LIBRARY ***********************
******** Description : This is library for TFT-9481x using STM32F103C8T6 uC 
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "pins_ext.h"

/* Define for connection */

#include "tft_defines.h"

//-------------end--------------------------------

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

/* Declare Function Prototype */
void startWrite(void);
void endWrite(void);
/******************************** User Function ***********************************/
uint16_t set_width(uint16_t width);
uint16_t set_height(uint16_t height);
uint16_t get_width(void);
uint16_t get_height(void);
void reset(void);
void tft_write_bus(uint8_t byte);
void tft_wr_pin_strob(void);
void tft_wr_pin_strob2(void);
void tft_write_reg16(uint16_t data);
void tft_write_color_fast(uint16_t data);
void tft_write_data16(uint16_t data);
void WriteCmdData(uint16_t cmd, uint16_t data);
void write_command_data(uint16_t cmd, uint16_t data);
void setRotation(uint8_t r);
void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);
void fastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void init_table16(const void *table, int16_t size);
void init_tft(void);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void tft_address_set(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
//=====================================================================================

#endif // TFT_BASE_HEADER_H
