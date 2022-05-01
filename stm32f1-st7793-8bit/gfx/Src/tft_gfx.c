#include "tft_gfx.h"
#include "tft_font.h"

#define DOT_SIZE 4

uint16_t color565_to_555(uint16_t color)
{
  return (color & 0xFFC0) | ((color & 0x1F) << 1) | ((color & 0x01));
}

uint16_t color555_to_565(uint16_t color)
{
  return (color & 0xFFC0) | ((color & 0x0400) >> 5) | ((color & 0x3F) >> 1);
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

void draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if (x < 0 || y < 0 || x >= tft_W || y >= tft_H) return;
  setAddrWindow(x, y, x, y);
  tft_write_color_fast(color);
}

void tft_draw_line(uint16_t column1, uint16_t row1, uint16_t column2, uint16_t row2, uint16_t color)
{
  uint16_t t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;
  delta_x=column2-column1;
  delta_y=row2-row1;
  uRow=column1;
  uCol=row1;
  if(delta_x>0)incx=1;
  else if(delta_x==0)incx=0;
  else
  {
    incx=-1;delta_x=-delta_x;
  }
  if(delta_y>0)incy=1;
  else if(delta_y==0)incy=0;
  else
  {
    incy=-1;delta_y=-delta_y;
  }
  if( delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  tft_draw_point(uRow,uCol,color);
  for(t=0;t<=distance+1;t++ )
  {
    tft_wr_pin_strob2();
    xerr+=delta_x ;
    yerr+=delta_y ;
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

void tft_draw_dot_line(uint16_t column1, uint16_t row1, uint16_t column2, uint16_t row2, uint16_t color)
{
  uint16_t t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;
  delta_x=column2-column1;
  delta_y=row2-row1;
  uRow=column1;
  uCol=row1;
  if(delta_x>0)incx=DOT_SIZE;
  else if(delta_x==0)incx=0;
  else
  {
    incx=-DOT_SIZE;delta_x=-delta_x;
  }
  if(delta_y>0)incy=DOT_SIZE;
  else if(delta_y==0)incy=0;
  else
  {
    incy=-DOT_SIZE;delta_y=-delta_y;
  }
  if( delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  for(t=0;t<=distance+1;t+=DOT_SIZE )
  {
    // tft_draw_point_slim(uRow,uCol,color);
    tft_draw_point(uRow,uCol,color);
    xerr+=delta_x ;
    yerr+=delta_y ;
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

void pushColors1(uint16_t * block, int16_t n, bool first)
{
  pushColors_any(0x0202, (uint8_t *)block, n, first, 0);
}

void pushColors2(uint8_t * block, int16_t n, bool first)
{
  pushColors_any(0x0202, (uint8_t *)block, n, first, 2);
}

void pushColors3(const uint8_t * block, int16_t n, bool first, bool bigend)
{
  pushColors_any(0x0202, (uint8_t *)block, n, first, bigend ? 3 : 1);
}

void pushColors_any(uint16_t cmd, uint8_t * block, int16_t n, uint8_t first, uint8_t flags)
{
  uint16_t color;
  uint8_t h, l;
  uint8_t isconst = flags & 1;
  uint8_t isbigend = (flags & 2) != 0;
  if (first)
  {
    tft_write_reg16(cmd);
  }
  while (n-- > 0)
  {
    if (isconst)
    {
      h = *block++;
      l = *block++;
    }
    else
    {
      h = (*block++);
      l = (*block++);
    }
    color = (isbigend) ? (h << 8 | l) : (l << 8 | h);
    tft_write_data16(color);
  }
}

//----------------------------------

void fillScreen(uint16_t color)
{
  fillRect(0, 0, tft_W, tft_H, color);
}

void tft_clear(uint16_t color)
{
  uint16_t i,j;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  tft_write_bus(color);
  GPIOB_Lo = 1<<CS;
  for(i=0;i<tft_W;i++)
  {
    for (j=0;j<tft_H;j++)
    {
      tft_wr_pin_strob2();
    }
  }
  GPIOB_Hi = 1<<CS;
}

void tft_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
  draw_pixel(x, y, color);
}

void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t size,uint16_t color)
{
  fillRect(x, y, size, size, color);
}

void tft_draw_point_bold(uint16_t x,uint16_t y,uint16_t color)
{
  fillRect(x, y, 2, 2, color);
}

void tft_draw_point_slim(uint16_t x,uint16_t y,uint16_t color)
{
  fillRect(x, y, 1, 1, color);
}

void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
  uint16_t i,j;
  tft_address_set(column1,row1,column2,row2);
  GPIOB_Lo = 1<<CS;
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      tft_write_color_fast(color);
    }
  }
  GPIOB_Hi = 1<<CS;
}

void tft_fill_fast(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
  uint16_t i,j;
  tft_address_set(column1,row1,column2,row2);
  tft_write_color_fast(color);
  GPIOB_Lo = 1<<CS;
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      GPIOB_Lo = 1<<WR;
      GPIOB_Hi = 1<<WR;
      __asm volatile ("nop");
      __asm volatile ("nop");
      __asm volatile ("nop");
      __asm volatile ("nop");
      GPIOB_Lo = 1<<WR;
      GPIOB_Hi = 1<<WR;
    }
  }
  GPIOB_Hi = 1<<CS;
}

void tft_fill_grid(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
  uint16_t y,x;
  tft_address_set(column1,row1,column2,row2);
  for(y=row1;y<=row2;y++)
  {
    for(x=column1;x<=column2;x++)
    {
      if(!(x%48))if(!(y% 8))tft_draw_point(x+8, y, COLOR_CYAN);
      if(!(x%8 )&&x<tft_W-16)if(!(y%48))tft_draw_point(x+8, y-24, COLOR_WHITE);
      tft_write_color_fast(color);
      GPIOB_Lo = 1<<CS;
      GPIOB_Lo = 1<<WR;
      GPIOB_Hi = 1<<WR;
      __asm volatile ("nop");
      __asm volatile ("nop");
      __asm volatile ("nop");
      __asm volatile ("nop");
      GPIOB_Lo = 1<<WR;
      GPIOB_Hi = 1<<WR;
    }
  }
  GPIOB_Hi = 1<<CS;
}

void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar26x48(column,row,*(string+i),TFT_STRING_MODE);
    column+=25;
    i++;
  }
}

void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar18x32(column,row,*(string+i),TFT_STRING_MODE);
    column+=17;
    i++;
  }
}

void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar14x24(column,row,*(string+i),TFT_STRING_MODE);
    column+=13;
    i++;
  }
}

void tft_puts8x16(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  while(*(string+i)!='\0')
  {
    tft_putchar(column,row,*(string+i),TFT_STRING_MODE);
    column+=8;
    i++;
  }
}

void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  uint16_t colortemp=POINT_COLOR;
  if(x>tft_W-16||y>tft_H-16)return;
  num=num-' ';
  tft_address_set(x,y,x+8-1,y+16-1);
  if(!mode)
  {
    for(pos=0;pos<16;pos++)
    {
      temp=asc2_1608[(uint16_t)num*16+pos];
      for(t=0;t<8;t++)
      {
        if(temp&0x01)POINT_COLOR=colortemp;
        else POINT_COLOR=BACK_COLOR;
        tft_write_color_fast(POINT_COLOR);
        temp>>=1;
      }
    }
  }
  else
  {
    for(pos=0;pos<16;pos++)
    {
      temp=asc2_1608[(uint16_t)num*16+pos];
      for(t=0;t<8;t++)
      {
        if(temp&0x01)tft_draw_point(x+t,y+pos,POINT_COLOR);
        temp>>=1;
      }
    }
  }
  POINT_COLOR=colortemp;
}

void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>tft_W-14)||(y>tft_H-24)) return;
  uint8_t i,j,k,temp;
  tft_address_set(x,y,x+14-1,y+24-1);
  for(i=0;i<24/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<14;k++)
      {
        temp=Consolas14x24[(data-' ')*(24/8)*14+k*(24/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_draw_point(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>tft_W-18)||(y>tft_H-32)) return;
  uint8_t i,j,k,temp;
  tft_address_set(x,y,x+18-1,y+32-1);
  for(i=0;i<32/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<18;k++)
      {
        temp=Consolas18x32[(data-' ')*(32/8)*18+k*(32/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_draw_point(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>tft_W-26)||(y>tft_H-48)) return;
  uint8_t i,j,k,temp;
  tft_address_set(x,y,x+26-1,y+48-1);
  for(i=0;i<48/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<26;k++)
      {
        temp=Consolas26x48[(data-' ')*(48/8)*26+k*(48/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_draw_point(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_draw_point(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

//===========================================================

/*
 * draw string
 */
void write_string(uint16_t x, uint16_t y, int8_t* label)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_YELLOW;
  tft_puts14x24(y,x,label,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw parameters
 */
void write_value(uint16_t x, uint16_t y, int16_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_YELLOW;

  char speed_value[16];
  utoa((int)value, speed_value, 10); // sprintf(speed_value,"%d",value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<10000) tft_fill(y,x+12*4,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000)  tft_fill(y,x+12*3,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<100)   tft_fill(y,x+12*2,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<10)    tft_fill(y,x+12*1,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  tft_puts14x24(y,x,(int8_t*)speed_value,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw parameters
 */
void write_value_x(uint16_t x, uint16_t y, uint16_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_YELLOW;

  char speed_value[16];
  // utoa((int)value, speed_value, 10);
  sprintf(speed_value,"%x",value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<10000) tft_fill(y,x+12*4,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000)  tft_fill(y,x+12*3,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<100)   tft_fill(y,x+12*2,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<10)    tft_fill(y,x+12*1,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  tft_puts14x24(y,x,(int8_t*)speed_value,TFT_STRING_MODE_BACKGROUND);
}

/*
 * draw parameters
 */
void write_value_hex(uint16_t x, uint16_t y, uint16_t value)
{
  BACK_COLOR=COLOR_BLACK;
  POINT_COLOR=COLOR_YELLOW;

  char speed_value[16];
  char str_dest[3] = "0x0";
  // utoa((int)value, speed_value, 10);
  sprintf(speed_value,"%x",value);
  strcat(str_dest, speed_value);
  // void tft_fill(row1,column1,row2,column2,color);
  if(value<10000) tft_fill(y,x+12*4,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<1000)  tft_fill(y,x+12*3,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<100)   tft_fill(y,x+12*2,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  if(value<10)    tft_fill(y,x+12*1,y+24,x+14*5,BACK_COLOR); // x1 y1 x2 y2
  tft_puts14x24(y,x,(int8_t*)str_dest,TFT_STRING_MODE_BACKGROUND);
}

//===========================================================

