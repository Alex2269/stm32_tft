#include "cursor.h"

extern uint16_t _cursor_x;
extern uint16_t _cursor_y;

uint16_t getCursorX(void)
{
  return _cursor_x;
}

uint16_t getCursorY(void)
{
  return _cursor_y;
}

void setCursorX(int16_t x)
{
  _cursor_x = x;
}

void setCursorY(int16_t y)
{
  _cursor_y = y;
}

void setCursor(int16_t x, int16_t y)
{
  _cursor_x = x;
  _cursor_y = y;
}
