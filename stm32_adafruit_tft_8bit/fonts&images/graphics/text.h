#ifndef text_H
#define text_H

/* Include Library */
#include "stdint.h"
#include "stdbool.h"

#include "ftoa_impl.h"

/* -------------------------------------------------------------------------------- */
/* -- DEFINES                                                                    -- */
/* -------------------------------------------------------------------------------- */
#ifndef NULL
   #define NULL ((void*) 0)
#endif

#ifndef swap
  #define swap(a, b)    { uint16_t  t=a; a=b; b=t; }
#endif

typedef enum
{
  FONT_TYPE_1BPP,
  FONT_TYPE_8BPP
} FONT_TYPE;

typedef struct
{
  unsigned char* p;
  FONT_TYPE font_type;
  int16_t char_width;
  int16_t char_height;
  uint16_t start_char;
  uint16_t end_char;
  uint8_t  *widths;
} UG_FONT;

void setTextSize(uint8_t s);
uint16_t getTextColor(void);
uint16_t getTextBgColor(void);
void setTextColor(uint32_t c);
void setTextBgColor(uint32_t c);
void UG_FONT_set(const UG_FONT* ug_font);
void UG_PutChar(char chr);
void UG_PutString(char* str);
int val2hex(int decnum);
void draw_value(int64_t value);
void draw_fvalue(float flt, uint8_t afterpoint);
void draw_digit(int decnum);
void write(uint8_t chr);

#endif // text_H


