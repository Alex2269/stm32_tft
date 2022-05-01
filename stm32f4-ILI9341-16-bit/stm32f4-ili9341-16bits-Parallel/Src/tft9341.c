#include "tft9341.h"
#include "tft_font.h"

/* Declare Private Variable */

GPIO_TypeDef* PORT_NAME_ARR[]=
{
  D0_PORT,D1_PORT,D2_PORT,D3_PORT,D4_PORT,D5_PORT,D6_PORT,D7_PORT,D8_PORT,D9_PORT,D10_PORT,D11_PORT,D12_PORT,D13_PORT,D14_PORT,D15_PORT
};

uint16_t PIN_NAME_ARR[]=
{
  D0_PIN,D1_PIN,D2_PIN,D3_PIN,D4_PIN,D5_PIN,D6_PIN,D7_PIN,D8_PIN,D9_PIN,D10_PIN,D11_PIN,D12_PIN,D13_PIN,D14_PIN,D15_PIN
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

void tft_write_bus(uint8_t high_byte,uint8_t low_byte)
{
  uint8_t i;
  for(i=0;i<8;i++)
  {
    if(high_byte&(0x01<<i))
    {
      pin_high(PORT_NAME_ARR[i+8],PIN_NAME_ARR[i+8]);
    }
    else
    {
      pin_low(PORT_NAME_ARR[i+8],PIN_NAME_ARR[i+8]);
    }
    if(low_byte&(0x01<<i))
    {
      pin_high(PORT_NAME_ARR[i],PIN_NAME_ARR[i]);
    }
    else
    {
      pin_low(PORT_NAME_ARR[i],PIN_NAME_ARR[i]);
    }
  }
  pin_low(WR_PORT,WR_PIN);
  pin_high(WR_PORT,WR_PIN);
}

void tft_write_data(uint16_t data)
{
  pin_high(RS_PORT,RS_PIN);
  tft_write_bus(data>>8,data&0x00ff);
}

void tft_write_reg(uint16_t data)
{
  pin_low(RS_PORT,RS_PIN);
  tft_write_bus(data>>8,data&0x00ff);
}

void tft_write_reg_data(uint16_t reg,uint16_t data)
{
  tft_write_reg(reg);
  tft_write_data(data);
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

void tft_init(void)
{
  pin_high(DSP_LED_GPIO_Port,DSP_LED_Pin);
  pin_high(CS_PORT,CS_PIN);
  pin_high(RD_PORT,RD_PIN);
  pin_high(WR_PORT,WR_PIN);
  pin_low(REST_PORT,REST_PIN);
  HAL_Delay(20);
  pin_high(REST_PORT,REST_PIN);
  HAL_Delay(20);
  pin_low(CS_PORT,CS_PIN);

  tft_write_reg(0x01);//Software Reset
  HAL_Delay(1);
  tft_write_reg(0xCB);//Power Control A
  tft_write_data(0x39);
  tft_write_data(0x2C);
  tft_write_data(0x00);
  tft_write_data(0x34);
  tft_write_data(0x02);
  HAL_Delay(1);
  tft_write_reg(0xCF);//Power Control B
  tft_write_data(0x00);
  tft_write_data(0xC1);
  tft_write_data(0x30);
  HAL_Delay(1);
  tft_write_reg(0xE8);//Driver timing control A
  tft_write_data(0x85);
  tft_write_data(0x00);
  tft_write_data(0x78);
  HAL_Delay(1);
  tft_write_reg(0xEA);//Driver timing control B
  tft_write_data(0x00);
  tft_write_data(0x00);
  HAL_Delay(1);
  tft_write_reg(0xED);//Power on Sequence control
  tft_write_data(0x64);
  tft_write_data(0x03);
  tft_write_data(0x12);
  tft_write_data(0x81);
  HAL_Delay(1);
  tft_write_reg(0xF7);//Pump ratio control
  tft_write_data(0x20);
  HAL_Delay(1);
  tft_write_reg(0xC0);//Power Control 1
  tft_write_data(0x10);
  HAL_Delay(1);
  tft_write_reg(0xC1);//Power Control 2
  tft_write_data(0x10);
  HAL_Delay(1);
  tft_write_reg(0xC5);//VCOM Control 1
  tft_write_data(0x3E);
  tft_write_data(0x28);
  HAL_Delay(1);
  tft_write_reg(0xC7);//VCOM Control 2
  tft_write_data(0x86);
  HAL_Delay(1);
  tft_write_reg(0x3A);//Pixel Format Set
  tft_write_data(0x55);//16bit
  HAL_Delay(1);
  tft_write_reg(0xB1);
  tft_write_data(0x00);
  tft_write_data(0x18);// Частота кадров 79 Гц
  HAL_Delay(1);
  tft_write_reg(0xB6);//Display Function Control
  tft_write_data(0x08);
  tft_write_data(0x82);
  tft_write_data(0x27);//320 строк
  HAL_Delay(1);
  tft_write_reg(0xF2);//Enable 3G
  tft_write_data(0x00);//не включаем
  HAL_Delay(1);
  tft_write_reg(0x26);//Gamma set
  tft_write_data(0x01);//Gamma Curve (G2.2) (Кривая цветовой гаммы)
  HAL_Delay(1);
  tft_write_reg(0xE0);//Positive Gamma Correction
  tft_write_data(0x0F);
  tft_write_data(0x31);
  tft_write_data(0x2B);
  tft_write_data(0x0C);
  tft_write_data(0x0E);
  tft_write_data(0x08);
  tft_write_data(0x4E);
  tft_write_data(0xF1);
  tft_write_data(0x37);
  tft_write_data(0x07);
  tft_write_data(0x10);
  tft_write_data(0x03);
  tft_write_data(0x0E);
  tft_write_data(0x09);
  tft_write_data(0x00);
  HAL_Delay(1);
  tft_write_reg(0xE1);//Negative Gamma Correction
  tft_write_data(0x00);
  tft_write_data(0x0E);
  tft_write_data(0x14);
  tft_write_data(0x03);
  tft_write_data(0x11);
  tft_write_data(0x07);
  tft_write_data(0x31);
  tft_write_data(0xC1);
  tft_write_data(0x48);
  tft_write_data(0x08);
  tft_write_data(0x0F);
  tft_write_data(0x0C);
  tft_write_data(0x31);
  tft_write_data(0x36);
  tft_write_data(0x0F);
  HAL_Delay(1);
  tft_write_reg(0x11);//Выйдем из спящего режим
  HAL_Delay(150);
  tft_SetRotation(0);
  HAL_Delay(1);
  tft_write_reg(0x29);//Включение дисплея
  tft_write_data(0x2C);
}

void tft_SetRotation(uint8_t r)
{
  tft_write_reg(0x36);
  switch(r)
  {
    case 0:
    tft_write_data(0x40|0x08);
    tft_W = 240;
    tft_H = 320;
    break;
    case 1:
    tft_write_data(0x20|0x08);
    tft_W = 320;
    tft_H = 240;
    break;
    case 2:
    tft_write_data(0x80|0x08);
    tft_W = 240;
    tft_H = 320;
    break;
    case 3:
    tft_write_data(0x40|0x80|0x20|0x08);
    tft_W = 320;
    tft_H = 240;
    break;
  }
}

void tft_clear(uint16_t color)
{
  uint16_t i,j;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  for(i=0;i<tft_W;i++)
  {
    for (j=0;j<tft_H;j++)
    {
      tft_write_data(color);
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
  int di;
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
    tft_write_data(temp);
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
    tft_write_data(temp);
  }
}

void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  //uint16_t x0=x;
  uint16_t colortemp=POINT_COLOR;
  if(x>tft_W-8||y>tft_H-16)return;
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
        tft_write_data(POINT_COLOR);
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
            tft_write_data(POINT_COLOR);
          }
          else
          {
            tft_write_data(BACK_COLOR);
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
            tft_write_data(POINT_COLOR);
          }
          else
          {
            tft_write_data(BACK_COLOR);
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
            tft_write_data(POINT_COLOR);
          }
          else
          {
            tft_write_data(BACK_COLOR);
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
