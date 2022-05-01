#include "init_R61581.h"
#include "tft_base.h"
#include "tft_font.h"

#define DOT_SIZE 8

/* Declare Private Variable */

GPIO_TypeDef* PORT_NAME_ARR[]=
{
  //D0_PORT,D1_PORT,D2_PORT,D3_PORT,D4_PORT,D5_PORT,D6_PORT,D7_PORT //,D8_PORT,D9_PORT,D10_PORT,D11_PORT,D12_PORT,D13_PORT,D14_PORT,D15_PORT
};

uint16_t PIN_NAME_ARR[]=
{
  //D0_PIN,D1_PIN,D2_PIN,D3_PIN,D4_PIN,D5_PIN,D6_PIN,D7_PIN //,D8_PIN,D9_PIN,D10_PIN,D11_PIN,D12_PIN,D13_PIN,D14_PIN,D15_PIN
};

uint16_t BACK_COLOR,POINT_COLOR;
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
    if(column>tft_W-16)
    {
      column=0;row+=16;
    }
    if(row>tft_H-16)
    {
      row=column=0;
    }
    tft_putchar(column,row,*(string+i),TFT_STRING_MODE);
    column+=8;
    i++;
  }
}

void tft_write_bus(uint8_t byte)
{
  //! GPIOA A0 A1 A2 A3 A4 A5 A6 A7
  //! TFT D0 D1 D2 D3 D4 D5 D6 D7
  //  GPIOA->ODR = byte;
  //! circular shift right of byte on 2 bits,
  //! to save from the cross direction of wires, on tft shield
  //! GPIOA A0 A1 A2 A3 A4 A5 A6 A7 (byte >> 2) | (byte << 6)
  //! TFT D2 D3 D4 D5 D6 D7 D0 D1
  GPIOA->ODR = (byte >> 2) | (byte << 6);
  // uint8_t shift;
  // shift=(byte >> 2) | (byte << 6);
  // for(uint8_t i=0;i<8;i++)
  // {
  //   (shift&(0x01<<i))?(GPIOA_Hi = 1<<i):(GPIOA_Lo = 1<<i);
  //   //(shift&(0x01<<i))?(pin_high(PORT_NAME_ARR[i],PIN_NAME_ARR[i])):(pin_low(PORT_NAME_ARR[i],PIN_NAME_ARR[i]));
  // }
}

// void tft_write_data(uint16_t data)
// {
//   pin_high(RS_PORT,RS_PIN);
//   tft_write_bus(data&0x00ff);
//   pin_low(WR_PORT,WR_PIN);
//   pin_high(WR_PORT,WR_PIN);
// }

// void tft_write_reg(uint16_t data)
// {
//   pin_low(RS_PORT,RS_PIN);
//   tft_write_bus(data&0x00ff);
//   pin_low(WR_PORT,WR_PIN);
//   pin_high(WR_PORT,WR_PIN);
// }

uint16_t tft_read_data(void)
{
  uint16_t data;
  // BUS_input();
  pin_high(GPIOB, DSP_RS_Pin);
  pin_high(GPIOB, DSP_WR_Pin);
  pin_low(GPIOB, DSP_RD_Pin);
  data=GPIOA->IDR & 0x00ff;
  pin_high(GPIOB, DSP_RD_Pin);
  // BUS_output();

  //return data;
  uint8_t shift;
  shift= ((data << 2) | (data >> 6));
  return shift;
}

void tft_write_color_fast(uint16_t data)
{
  pin_high(GPIOB, DSP_RS_Pin);
  tft_write_bus(data>>8);
  pin_low(GPIOB, DSP_WR_Pin);
  pin_high(GPIOB, DSP_WR_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(GPIOB, DSP_WR_Pin);
  pin_high(GPIOB, DSP_WR_Pin);
}

void tft_write_data(uint16_t data)
{
  pin_high(GPIOB, DSP_RS_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(GPIOB, DSP_WR_Pin);
  pin_high(GPIOB, DSP_WR_Pin);
}

void tft_write_reg(uint16_t data)
{
  pin_low(GPIOB, DSP_RS_Pin);
  tft_write_bus(data&0x00ff);
  pin_low(GPIOB, DSP_WR_Pin);
  pin_high(GPIOB, DSP_WR_Pin);
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

void init_table8(const void *table, int16_t size)
{
  uint8_t *p = (uint8_t *) table;
  while (size > 0)
  {
    uint8_t cmd = *p++;
    uint8_t len = *p++;
    if (cmd == TFTLCD_DELAY8)
    {
      LL_mDelay(len);
      len = 0;
    }
    else
    {
      tft_write_reg(cmd);
      for (uint8_t i = 0; i < len; i++)
      if(len)
      {
        tft_write_data(*p++);
      }
    }
    size -= len + 2 * sizeof(int8_t);
  }
}

void init_table16(const void *table, int16_t size)
{
  uint16_t *p = (uint16_t *) table;
  while (size > 0)
  {
    uint16_t dat;
    uint16_t cmd = *p++;
    uint16_t len = *p++;
    if (cmd == TFTLCD_DELAY)
    {
      LL_mDelay(len);
      len = 0;
    }
    else
    {
      tft_write_reg(cmd);
      for (uint16_t i = 0; i < len; i++)
      if(len)
      {
        dat = *p++;
        tft_write_data(dat>>8);
        tft_write_data(dat&0x00ff);
      }
    }
    size -= len + 2 * sizeof(int16_t);
  }
}

void tft_init(void)
{
  pin_high(DSP_LED_GPIO_Port,DSP_LED_Pin);
  pin_high(CS_PORT,CS_PIN);
  pin_high(RD_PORT,RD_PIN);
  pin_high(WR_PORT,WR_PIN);
  pin_low(REST_PORT,REST_PIN);
  LL_mDelay(20);
  pin_high(REST_PORT,REST_PIN);
  LL_mDelay(20);
  pin_low(CS_PORT,CS_PIN);

  init_table8(&init_array_R61581_8, sizeof(init_array_R61581_8)); // #include "init_R61581.h"
  //init_table16(&init_array_R61581_16, sizeof(init_array_R61581_16)); // #include "init_R61581.h"
  //init_table16(&init_array_ili9325_16, sizeof(init_array_ili9325_16)); // #include "init_R61581.h"
}

void tft_SetRotation(uint8_t r)
{
  tft_write_reg(0x36);
  if(r == 0)
  {
    tft_write_data(0x40|0x08);
    tft_W = 320;
    tft_H = 480;
  }
  if(r == 1)
  {
    tft_write_data(0x20|0x08);
    tft_W = 480;
    tft_H = 320;
  }
  if(r == 2)
  {
    tft_write_data(0x80|0x08);
    tft_W = 320;
    tft_H = 480;
  }
  if(r == 3)
  {
    tft_write_data(0x40|0x80|0x20|0x08);
    tft_W = 480;
    tft_H = 320;
  }
}

void tft_clear(uint16_t color)
{
  uint16_t i,j;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  tft_write_data(color>>8);
  tft_write_data(color&0x00ff);
  for(i=0;i<tft_W;i++)
  {
    for (j=0;j<tft_H;j++)
    {
      pin_low(GPIOB, DSP_WR_Pin);
      pin_high(GPIOB, DSP_WR_Pin);
      pin_low(GPIOB, DSP_WR_Pin);
      pin_high(GPIOB, DSP_WR_Pin);
    }
  }
}

void tft_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
  tft_address_set(x,y,x,y);
  tft_write_data(color>>8);
  tft_write_data(color&0x00ff);
}

void tft_draw_point_big(uint16_t x,uint16_t y,uint16_t color)
{
  int16_t i,j;
  tft_address_set(x-1,y-1,x+1,y+1);
  for(i=x-1;i<=x+1;i++)
  {
    for(j=y-1;j<=y+1;j++)
    {
      tft_write_data(color>>8);
      tft_write_data(color&0x00ff);
    }
  }
}

void tft_draw_point_big2(uint16_t x,uint16_t y,uint16_t color)
{
  int16_t i,j;
  tft_address_set(x-2,y-2,x+2,y+2);
  for(i=x-2;i<=x+2;i++)
  {
    for(j=y-2;j<=y+2;j++)
    {
      tft_write_data(color>>8);
      tft_write_data(color&0x00ff);
    }
  }
}

void tft_draw_point_bold(uint16_t x,uint16_t y,uint16_t color)
{
  tft_fill(x-1,y-1,x+1,y+1,color);
}

void tft_draw_point_slim(uint16_t x,uint16_t y,uint16_t color)
{
  tft_fill(x,y,x,y,color);
}

void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color)
{
  uint16_t i,j;
  tft_address_set(column1,row1,column2,row2);
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      tft_write_data(color>>8);
      tft_write_data(color&0x00ff);
    }
  }
}

void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color)
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
  for(t=0;t<=distance+1;t++ )
  {
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

void tft_draw_dot_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color)
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
    tft_draw_point_slim(uRow,uCol,color);
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

void tft_draw_line_slim(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color)
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
  for(t=0;t<=distance+1;t++ )
  {
    tft_draw_point_slim(uRow,uCol,color);
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

void tft_draw_line_bold(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color)
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
  for(t=0;t<=distance+1;t++ )
  {
    tft_draw_point_bold(uRow,uCol,color);
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

void tft_draw_rectangle(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2,uint16_t color)
{
  tft_draw_line(row1,column1,row1,column2,color);
  tft_draw_line(row1,column2,row2,column2,color);
  tft_draw_line(row2,column2,row2,column1,color);
  tft_draw_line(row2,column1,row1,column1,color);
}

void tft_draw_circle(uint16_t row,uint16_t column,uint16_t radian,uint16_t color)
{
  int a,b;
  int32_t di;
  a=0;b=radian;
  di=3-(radian<<1);
  while(a<=b)
  {
    tft_draw_point(column-b,row-a,color); //3
    tft_draw_point(column+b,row-a,color); //0
    tft_draw_point(column-a,row+b,color); //1
    tft_draw_point(column-b,row-a,color); //7
    tft_draw_point(column-a,row-b,color); //2
    tft_draw_point(column+b,row+a,color); //4
    tft_draw_point(column+a,row-b,color); //5
    tft_draw_point(column+a,row+b,color); //6
    tft_draw_point(column-b,row+a,color);
    a++;
    //Bresenham
    if(di<0)di +=4*a+6;
    else
    {
      di+=10+4*(a-b);
      b--;
    }
    tft_draw_point(column+a,row+b,color);
  }
}

void tft_puts_image_pos(const unsigned char* image_arr, uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y)
{
  uint32_t i;
  uint16_t temp;
  uint8_t high_byte,low_byte;
  tft_address_set(x,y,x+size_x-1,y+size_y-1);
  for(i=x;i<(size_x*size_y);i++)
  {
    low_byte=*(image_arr+i*2);
    high_byte=*(image_arr+i*2+1);
    temp=((high_byte)<<8|(low_byte));
    tft_write_data(temp>>8);
    tft_write_data(temp&0x00ff);
  }
}

void tft_puts_image(const unsigned char* image_arr)
{
  uint32_t i;
  uint16_t temp;
  uint8_t high_byte,low_byte;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  for(i=0;i<(tft_W*tft_H);i++)
  {
    low_byte=*(image_arr+i*2);
    high_byte=*(image_arr+i*2+1);
    temp=((high_byte)<<8|(low_byte));
    tft_write_data(temp>>8);
    tft_write_data(temp&0x00ff);
  }
}

void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  //uint16_t x0=x;
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
