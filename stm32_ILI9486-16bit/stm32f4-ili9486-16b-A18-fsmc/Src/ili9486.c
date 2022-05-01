#include "ili9486.h"
#include "tft_font.h"

uint16_t BACK_COLOR,POINT_COLOR;
extern RNG_HandleTypeDef hrng;
uint16_t tft_W = 0;
uint16_t tft_H = 0;
uint32_t dtt=0;
uint32_t TextColor;
uint32_t BackColor;
uint8_t *pchar, *str2;

void tft_Delay(volatile uint32_t delay)
{
  uint32_t i;
  for(i=0;i<delay;i++);
}

__STATIC_INLINE void DelayMicro(uint32_t __IO micros)
{
  micros *=(SystemCoreClock/1000000)/5;
  while(micros--);
}

void tft_color_fonts(void)
{
  BackColor=BLACK;
  TextColor=GREEN;
}

void tft_write_data(volatile uint16_t data)
{
  // pin_high(FSMC_RS_GPIO_Port,FSMC_RS_Pin);
  // tft_Delay(4);
  ADDR_DATA = (data);
}

void tft_write_reg(volatile uint16_t data)
{
  // pin_low(FSMC_RS_GPIO_Port,FSMC_RS_Pin);
  // tft_Delay(4);
  ADDR_CMD = (data);
}

uint32_t tft_ReadReg(uint8_t r)
{
  uint32_t id = 0;
  tft_write_reg(r);
  DelayMicro(50);
  for (uint8_t i = 0; i < 4; i++)
  {
    id <<= 8; id |= ADDR_DATA;
  }
  DelayMicro(150);
  return id;
}

void tft_SetRotation(uint8_t r)
{
  tft_write_reg(0x36);
  switch(r)
  {
    case 0:
    {
      tft_write_data(0x40|0x08);
      tft_W = 320;
      tft_H = 480;
    }
    break;
    case 1:
    {
      tft_write_data(0x20|0x08);
      tft_W = 480;
      tft_H = 320;
    }
    break;
    case 2:
    {
      tft_write_data(0x80|0x08);
      tft_W = 320;
      tft_H = 480;
    }
    break;
    case 3:
    {
      tft_write_data(0x40|0x80|0x20|0x08);
      tft_W = 480;
      tft_H = 320;
    }
    break;
  }
}

void tft_Flood(uint16_t color,uint32_t len)
{
  uint16_t blocks;
  uint8_t i;
  tft_write_reg(0x2C);
  tft_write_data(color);
  len--;
  blocks=(uint16_t)(len/64);
  while(blocks--)
  {
    i=16;
    do
    {
      tft_write_data(color);
      tft_write_data(color);
      tft_write_data(color);
      tft_write_data(color);
      tft_write_data(color);
    }
    while(--i);
  }
  for(i=(uint8_t)len&63;i--;)
  {
    tft_write_data(color);
  }
}

void tft_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_write_reg(0x2a);
  tft_write_data(x1>>8);
  tft_write_data(x1&0x00ff);
  tft_write_data(x2>>8);
  tft_write_data(x2&0x00ff);
  DelayMicro(1);
  tft_write_reg(0x2b);
  tft_write_data(y1>>8);
  tft_write_data(y1&0x00ff);
  tft_write_data(y2>>8);
  tft_write_data(y2&0x00ff);
  DelayMicro(1);
  tft_write_reg(0x2c);
}

void tft_FillScreen(uint16_t color)
{
  tft_SetAddrWindow(0,0,tft_W-1,tft_H-1);
  tft_Flood(color,(long)tft_W*(long)tft_H);
}

void tft_FillRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_SetAddrWindow(x1,y1,x2,y2);
  tft_Flood(color,(uint16_t)(x2-x1+1)*(uint16_t)(y2-y1+1));
}

void tft_DrawPixel(int64_t x, int64_t y, uint16_t color)
{
  if((x<0)||(y<0)||(x>=tft_W)||(y>=tft_H)) return;
  tft_SetAddrWindow(x,y,x,y);
  tft_write_reg(0x2C);
  tft_write_data(color);
}

void tft_DrawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int64_t steep = abs(y2-y1)>abs(x2-x1);
  if(steep)
  {
    swap(x1,y1);
    swap(x2,y2);
  }
  if(x1>x2)
  {
    swap(x1,x2);
    swap(y1,y2);
  }
  int64_t dx,dy;
  dx=x2-x1;
  dy=abs(y2-y1);
  int64_t err=dx/2;
  int64_t ystep;
  if(y1<y2) ystep=1;
  else ystep=-1;
  for(;x1<=x2;x1++)
  {
    if(steep) tft_DrawPixel(y1,x1,color);
    else tft_DrawPixel(x1,y1,color);
    err-=dy;
    if(err<0)
    {
      y1 += ystep;
      err=dx;
    }
  }
}

void tft_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_DrawLine(color,x1,y1,x2,y1);
  tft_DrawLine(color,x2,y1,x2,y2);
  tft_DrawLine(color,x1,y1,x1,y2);
  tft_DrawLine(color,x1,y2,x2,y2);
}

void tft_DrawCircle(uint16_t x0, uint16_t y0, int64_t r, uint16_t color)
{
  int64_t f = 1-r;
  int64_t ddF_x=1;
  int64_t ddF_y=-2*r;
  int64_t x = 0;
  int64_t y = r;
  tft_DrawPixel(x0,y0+r,color);
  tft_DrawPixel(x0,y0-r,color);
  tft_DrawPixel(x0+r,y0,color);
  tft_DrawPixel(x0-r,y0,color);
  while (x<y)
  {
    if (f>=0)
    {
      y--;
      ddF_y+=2;
      f+=ddF_y;
    }
    x++;
    ddF_x+=2;
    f+=ddF_x;
    tft_DrawPixel(x0+x,y0+y,color);
    tft_DrawPixel(x0-x,y0+y,color);
    tft_DrawPixel(x0+x,y0-y,color);
    tft_DrawPixel(x0-x,y0-y,color);
    tft_DrawPixel(x0+y,y0+x,color);
    tft_DrawPixel(x0-y,y0+x,color);
    tft_DrawPixel(x0+y,y0-x,color);
    tft_DrawPixel(x0-y,y0-x,color);
  }
}

uint16_t tft_RandColor(void)
{
  return HAL_RNG_GetRandomNumber(&hrng)&0x0000FFFF;
}

void tft_reset(void)
{
  pin_low(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay(5);
  pin_high(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
}

void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_write_reg(0x2a);
  tft_write_data(x1>>8);
  tft_write_data(x1&0x00ff);
  tft_write_data(x2>>8);
  tft_write_data(x2&0x00ff);
  tft_write_reg(0x2b);
  tft_write_data(y1>>8);
  tft_write_data(y1&0x00ff);
  tft_write_data(y2>>8);
  tft_write_data(y2&0x00ff);
  tft_write_reg(0x2c);
}

void tft_ini(void)
{
  char str[11];
  tft_reset();
  // HAL_GPIO_WritePin(STM_LED1_GPIO_Port, STM_LED1_Pin, GPIO_PIN_RESET); // LED1
  // HAL_GPIO_WritePin(STM_LED2_GPIO_Port, STM_LED2_Pin, GPIO_PIN_RESET); // LED2
  HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET); // LIGHT
  HAL_Delay(500);
  dtt = tft_ReadReg(0xD3);
  LCD_Clear();
  LCD_SetPos(0,0);
  sprintf(str,"0x%08lX",(unsigned long)dtt);
  LCD_String(str);
  tft_write_reg(0x01);//Software Reset
  DelayMicro(1);
  ///!
  tft_write_reg(0x0b);
  tft_write_data(0x00);
  tft_write_data(0x00);
  // Sleep OUT
  tft_write_reg(0x11);
  // Interface Pixel Format
  tft_write_reg(0x3a);
  tft_write_data(0x55); //0x66 5-6-5 / 55 6-6-6
  // Memory Access Control (
  tft_write_reg(0x36);
  tft_write_data(0x48); // MX, BGR == rotation 0
  // Power Control 3 (For Normal Mode)
  tft_write_reg(0xc2);
  tft_write_data(0x44);
  // VCOM Control
  tft_write_reg(0xc5);
  tft_write_data(0x00);
  tft_write_data(0x00);
  tft_write_data(0x00);
  tft_write_data(0x00);
  // PGAMCTRL(Positive Gamma Control)
  tft_write_reg(0xe0);
  tft_write_data(0x0F);
  tft_write_data(0x1F);
  tft_write_data(0x1C);
  tft_write_data(0x0C);
  tft_write_data(0x0F);
  tft_write_data(0x08);
  tft_write_data(0x48);
  tft_write_data(0x98);
  tft_write_data(0x37);
  tft_write_data(0x0A);
  tft_write_data(0x13);
  tft_write_data(0x04);
  tft_write_data(0x11);
  tft_write_data(0x0D);
  tft_write_data(0x00);
  // NGAMCTRL (Negative Gamma Correction)
  tft_write_reg(0xe1);
  tft_write_data(0x0F);
  tft_write_data(0x32);
  tft_write_data(0x2E);
  tft_write_data(0x0B);
  tft_write_data(0x0D);
  tft_write_data(0x05);
  tft_write_data(0x47);
  tft_write_data(0x75);
  tft_write_data(0x37);
  tft_write_data(0x06);
  tft_write_data(0x10);
  tft_write_data(0x03);
  tft_write_data(0x24);
  tft_write_data(0x20);
  tft_write_data(0x00);
  // Digital Gamma Control 1
  tft_write_reg(0xe2);
  tft_write_data(0x0F);
  tft_write_data(0x32);
  tft_write_data(0x2E);
  tft_write_data(0x0B);
  tft_write_data(0x0D);
  tft_write_data(0x05);
  tft_write_data(0x47);
  tft_write_data(0x75);
  tft_write_data(0x37);
  tft_write_data(0x06);
  tft_write_data(0x10);
  tft_write_data(0x03);
  tft_write_data(0x24);
  tft_write_data(0x20);
  tft_write_data(0x00);
  tft_write_reg(0x11); // Sleep OUT
  HAL_Delay(150);
  tft_SetRotation(0);
  tft_write_reg(0x29); // Display ON
  tft_write_data(0x2C);
  ///!
  tft_color_fonts();
}

void tft_clear(uint16_t color)
{
  uint16_t i,j;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  for(i=0;i<tft_W;i++)
  {
    for (j=0;j<tft_H;j++)
    {
      // pin_high(RS_PORT,RS_PIN);
      // tft_write_data(color>>8);
      // tft_write_data(color&0x00ff);
      tft_write_data(color);
      // pin_low(RS_PORT,RS_PIN);
    }
  }
}

void tft_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
  tft_address_set(x,y,x,y);
  tft_write_data(color);
}

void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t color)
{
  tft_fill(x-1,y-1,x+1,y+1,color);
}

void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
  uint16_t i,j;
  tft_address_set(column1,row1,column2,row2);
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      tft_write_data(color);
    }
  }
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
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
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
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
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
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            tft_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}
