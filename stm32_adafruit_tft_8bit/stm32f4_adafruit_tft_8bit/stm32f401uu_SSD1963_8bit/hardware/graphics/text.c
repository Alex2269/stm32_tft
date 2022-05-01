#include "text.h"
#include "SSD1963.h"
#include "io_SSD1963.h"

UG_FONT* font;

static uint16_t _textsize = 1;
static uint32_t _fontColor = 0xFFFFFF;
static uint32_t _fontBgColor = 0x000000;

void setTextSize(uint8_t s)
{
  _textsize = (s > 0) ? s : 1;
}

uint16_t getTextColor(void)
{
  return _fontColor;
}

uint16_t getTextBgColor(void)
{
  return _fontBgColor;
}

void setTextColor(uint32_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  _fontColor = c;
}

void setTextBgColor(uint32_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  _fontBgColor = c;
}

void UG_FONT_set(const UG_FONT* ug_font)
{
  font = ug_font;
}

void UG_PutChar(char chr)
{
  uint16_t x = getCursorX();
  uint16_t y = getCursorY();
  uint16_t now_x, now_y;
  uint16_t i,j,k,c,bn,actual_char_width;
  uint8_t b,bt;
  uint32_t index;
  uint32_t color;

  void(*push_pixel)(uint32_t);

  bt = (uint8_t)chr;

  if (bt < font->start_char || bt > font->end_char) return;

  bn = font->char_width;
  if ( !bn ) return;
  bn >>= 3;
  if ( font->char_width % 8 ) bn++;
  actual_char_width = (font->widths ? font->widths[bt - font->start_char] : font->char_width);

  /*Not accelerated output*/
  Display_WindowSet(x,x+actual_char_width-1,y,y+font->char_height);
  if (font->font_type == FONT_TYPE_1BPP)
  {
    index = (bt - font->start_char)* font->char_height * bn;
    for( j=0;j<font->char_height;j++ )
    {
      now_x = x;
      c=actual_char_width;
      for( i=0;i<bn;i++ )
      {
        b = font->p[index++];
        for( k=0;(k<8) && c;k++ )
        {
          if( b & 0x01 )
          {
            // writePixel(now_x,now_y, _fontColor);
            // fillRect(now_x,now_y,_textsize,_textsize,_fontColor);
            Display_WriteData24(_fontColor);
          }
          else
          {
            // writePixel(now_x,now_y, _fontBgColor);
            // fillRect(now_x,now_y,_textsize,_textsize,_fontBgColor);
            Display_WriteData24(_fontBgColor);
          }
          b >>= 1;
          now_x += _textsize;
          c--;
        }
      }
      now_y += _textsize;
    }
  }
  else if (font->font_type == FONT_TYPE_8BPP)
  {
    index = (bt - font->start_char)* font->char_height * font->char_width;
    for( j=0;j<font->char_height;j++ )
    {
      now_x = x;
      for( i=0;i<actual_char_width;i++ )
      {
        b = font->p[index++];
        color = (((_fontColor & 0xFF) * b + (_fontBgColor & 0xFF) * (256 - b)) >> 8) & 0xFF | // blue component
        (((_fontColor & 0xFF00) * b + (_fontBgColor & 0xFF00) * (256 - b)) >> 8) & 0xFF00| // green component
        (((_fontColor & 0xFF0000) * b + (_fontBgColor & 0xFF0000) * (256 - b)) >> 8) & 0xFF0000; // red component
        // writePixel(now_x,now_y, _fontBgColor);
        // fillRect(now_x,now_y,_textsize,_textsize,color);
        Display_WriteData24(color);
        now_x += _textsize;;
      }
      index += font->char_width - actual_char_width;
      now_y += _textsize;
    }
  }
}

void UG_PutString(char* str)
{
  uint8_t cw;
  char chr;

  while ( *str != 0 )
  {
    chr = *str++;

    if(chr == '\n')
    {
     setCursorY(getCursorY() + _textsize * font->char_height);
     setCursorX(0);
    }

    if (chr < font->start_char || chr > font->end_char) continue;

    cw = font->widths ? font->widths[chr - font->start_char] : font->char_width;

    UG_PutChar(chr);
    setCursorX(getCursorX() +  cw * _textsize); // + gui->char_h_space;
  }
}

int val2hex(int decnum)
{
  int rem, i=0;
  char hexnum[50];
  while(decnum!=0)
  {
    rem = decnum%16;
    if(rem<10)
    rem = rem+48;
    else
    rem = rem+55;
    hexnum[i] = rem;
    i++;
    decnum = decnum/16;
  }
  for(i=i-1; i>=0; i--)
  {
    UG_PutChar(hexnum[i]);
    setCursorX(getCursorX() + 20);
  }
  // UG_PutString(hexnum);
}

void draw_value(int64_t value)
{
  uint8_t max_mv[16];
  utoa_local((int64_t)value, max_mv, 10);

  UG_PutString(max_mv);

  // clear spase old value
  char letter= 'X'; // X letter selected due to classic width for digits
  volatile uint8_t letter_width;
  letter_width = (font->widths ? font->widths[letter - font->start_char] : font->char_width); // get width for curent letter

  // shading previous characters background color
  if(value<1000) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 1, font->char_height * _textsize , _fontBgColor);
  if(value<100) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 2, font->char_height * _textsize , _fontBgColor);
  if(value<10) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 3, font->char_height * _textsize , _fontBgColor);
}

void draw_fvalue(float flt, uint8_t afterpoint)
{
  uint8_t res[24];
  ftoa_local(flt, res, afterpoint);
  if(flt>=0) write('+');
  if(flt<0)
  {
    write('-');
    flt = fabsf(flt);
  }

  UG_PutString(res);

  // clear spase old value
  char letter= 'X'; // X letter selected due to classic width for digits
  volatile uint8_t letter_width; // width for curent letter
  letter_width = (font->widths ? font->widths[letter - font->start_char] : font->char_width); // get width for curent letter

  // shading previous characters background color
  if(flt<1000) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 1, font->char_height * _textsize , _fontBgColor);
  if(flt<100) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 2, font->char_height * _textsize , _fontBgColor);
  if(flt<10) fillRect(getCursorX(), getCursorY(), letter_width * _textsize * 3, font->char_height * _textsize , _fontBgColor);
}

void draw_digit(int decnum)
{
    int rem, i=0;
    char hexnum[50];

    while(decnum!=0)
    {
        rem = decnum%16;
        if(rem<10)
            rem = rem+48;
        else
            rem = rem+55;
        hexnum[i] = rem;
        i++;
        decnum = decnum/16;
    }
    for(i=i-1; i>=0; i--)
    {
        UG_PutChar(hexnum[i]);
        setCursorX(getCursorX() + 20);
    }
   // UG_PutString(hexnum);
}

void write(uint8_t chr)
{
  // if(chr== ',') return;
  // if(chr< '0') return;

  uint16_t actual_char_width;

  if (chr < font->start_char || chr > font->end_char) return;

  actual_char_width = (font->widths ? font->widths[chr - font->start_char] : font->char_width);

  if(chr== '\n')
  {
    setCursorY(getCursorY() + font->char_height * _textsize);
    setCursorX(0);
  }
  else if(chr== '\r')
  {
    // skip em
  }
  else
  {
    if(setCursorX(getCursorX() + actual_char_width * _textsize) >= Display_Get_WIDTH())
    {
      // Heading off edge?
      setCursorX(0); // Reset x to zero
      setCursorY(getCursorY() + actual_char_width * _textsize); // Advance y one line
    }
    setCursor(getCursorX(), getCursorY());
    UG_PutChar(chr);
    setCursorX(getCursorX() + actual_char_width * _textsize);
  }
}

