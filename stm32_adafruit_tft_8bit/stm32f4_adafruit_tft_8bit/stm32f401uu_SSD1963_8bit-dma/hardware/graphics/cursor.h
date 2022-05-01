#ifndef cursor_H
#define cursor_H

/* Include Library */
#include "stdint.h"

static uint16_t _cursor_x = 0;
static uint16_t _cursor_y = 0;

uint16_t getCursorX(void);
uint16_t getCursorY(void);
void setCursorX(int16_t x);
void setCursorY(int16_t y);
void setCursor(int16_t x, int16_t y);


#endif // cursor_H


