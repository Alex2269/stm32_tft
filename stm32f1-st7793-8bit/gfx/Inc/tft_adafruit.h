#ifndef TFT_adafruit_H
#define TFT_adafruit_H

/************************ TFT adafruit LIBRARY ***************************
******** Description : This is library for TFT using STM32F103C8T6 uC ****
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "pins_ext.h"

/* Define for connection */

#include "../pgmspace.h"
#include "tft_defines.h"
#include "tft_base.h"
#include "gfxfont.h"

//-------------end--------------------------------

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

const int16_t WIDTH, HEIGHT; // This is the 'raw' display w/h - never changes
int16_t _width, _height, // Display w/h as modified by current rotation
cursor_x, cursor_y;
uint16_t textcolor, textbgcolor;
uint8_t textsize, rotation;
bool wrap; // If set, 'wrap' text at right edge of display
bool _cp437; // If set, use correct CP437 charset (default is off)
GFXfont *gfxFont;
GFXfont *font;

/* Declare Function Prototype */

/******************************** User Function ***********************************/

//----------------------------------------------------------
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void startWrite(void);
void endWrite(void);
void writePixel(int16_t x, int16_t y, uint16_t color);
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
// void fastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
void drawBitmap2(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
void drawBitmap3(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void drawBitmap4(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
void drawGrayscaleBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
void drawGrayscaleBitmap2(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
void drawGrayscaleBitmap3(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
void drawGrayscaleBitmap4(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
void drawRGBBitmap1(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
void drawRGBBitmap2(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
void drawRGBBitmap3(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
void drawRGBBitmap4(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void write(uint8_t c);
void setCursor(int16_t x, int16_t y);
int16_t getCursorX(void);
int16_t getCursorY(void);
void setTextSize(uint8_t s);
void setTextColor(uint16_t c);
void setTextColor_bg(uint16_t c, uint16_t bg);
void setFont(const GFXfont *f);
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void getTextBounds2(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
int16_t width(void);
int16_t height(void);

#endif // TFT_adafruit_H
