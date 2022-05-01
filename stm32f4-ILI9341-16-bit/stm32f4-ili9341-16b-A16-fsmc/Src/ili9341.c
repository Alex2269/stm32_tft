#include "ili9341.h"
#include "tft_font.h"

uint16_t BACK_COLOR,POINT_COLOR;

extern RNG_HandleTypeDef hrng;
uint16_t tft_W = 0;
uint16_t tft_H = 0;
uint32_t dtt=0;
uint32_t TextColor;
uint32_t BackColor;
uint8_t *pchar, *str2;
extern FIL MyFile;

typedef struct
{
  uint16_t TextColor;
  uint16_t BackColor;
} LCD_DrawPropTypeDef;

LCD_DrawPropTypeDef lcdprop;

void tft_Delay(volatile uint32_t delay)
{
  uint32_t i;
  for(i=0;i<delay;i++);
}

void tft_write_data(volatile uint16_t data)
{
  pin_high(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_Delay(3);
  ADDR_DATA = (data);
}

void tft_write_reg(volatile uint16_t data)
{
  pin_low(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_Delay(3);
  ADDR_CMD = (data);
}

uint32_t tft_ReadReg(uint8_t r)
{
  uint32_t id = 0;
  tft_write_reg(r);
  tft_Delay(50);
  for (uint8_t i = 0; i < 4; i++) { id <<= 8; id |= ADDR_DATA; }
  tft_Delay(150);
  return id;
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

void tft_Flood(uint16_t color,uint32_t len)
{
  uint16_t blocks;
  uint8_t i;
  tft_write_reg(0x2C);
  tft_write_data(color);
  len--;
  blocks=(uint16_t)(len/64); // 64pixels/blocks
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
  // Fill any remaining pixels (1 to 64);
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
  tft_Delay(1);
  tft_write_reg(0x2b);
  tft_write_data(y1>>8);
  tft_write_data(y1&0x00ff);
  tft_write_data(y2>>8);
  tft_write_data(y2&0x00ff);
  tft_Delay(1);
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

void tft_FontsIni(void)
{
  lcdprop.BackColor=BLACK;
  lcdprop.TextColor=GREEN;
}

void tft_DrawBitmap(uint16_t x,uint16_t y, char *s)
{
  uint16_t i,h;
  uint32_t index=0, width=0, height=0, bitpixel=0;
  FRESULT res; // FatFs function common result code
  uint32_t bytesread;
  uint32_t tmpcolor = 0;
  uint8_t *bufbmp = NULL;
  f_close(&MyFile);
  if(f_open(&MyFile,s,FA_READ)!=FR_OK)
  {
    Error_Handler();
  }
  else
  {
    bufbmp = (uint8_t*) malloc(100);
    res = f_read(&MyFile, bufbmp, 90,(void*)&bytesread);
    if((bytesread==0)||(res!=FR_OK))
    {
      Error_Handler();
    }
    else
    {
      index=bufbmp[10]; // адрес начала данных в файле
      index|=bufbmp[11]<<8;
      index|=bufbmp[12]<<16;
      index|=bufbmp[13]<<24;
      width=bufbmp[18]; // ширина растра
      width|=bufbmp[19]<<8;
      width|=bufbmp[20]<<16;
      width|=bufbmp[21]<<24;
      height=bufbmp[22]; // высота растра
      height|=bufbmp[23]<<8;
      height|=bufbmp[24]<<16;
      height|=bufbmp[25]<<24;
      bitpixel=bufbmp[28]; // формат пикселя (бит/пиксель)
      bitpixel|=bufbmp[29]<<8;
      for(h=0;h<height;h++)
      {
        for(i=0;i<width;i++)
        {
          switch(bitpixel)
          {
            case 24:
            f_lseek(&MyFile,index+((height-h-1)*width*3)+(i*3));
            res = f_read(&MyFile, bufbmp, 3,(void*)&bytesread);
            if((bytesread==0)||(res!=FR_OK))
            {
              Error_Handler();
            }
            else
            {
              tmpcolor=bufbmp[0];
              tmpcolor|=bufbmp[1]<<8;
              tmpcolor|=bufbmp[2]<<16;
            }
            break;
          }
          if(((i+x)<width)|((h+y)<height))
          tft_DrawPixel(i+x,h+y,(uint16_t)convert24to16(tmpcolor));
        }
      }
      f_close(&MyFile);
    }
    free(bufbmp);
  }
}

uint16_t tft_RandColor(void)
{
  return HAL_RNG_GetRandomNumber(&hrng)&0x0000FFFF;
}

void tft_reset(void)
{
  tft_write_reg(0x01); // Software Reset
  for(uint8_t i=0;i<3;i++) tft_write_data(0x00);
  HAL_Delay(10);
  pin_low(RST_PORT,RST_PIN);
  HAL_Delay(50);
  pin_high(RST_PORT,RST_PIN);
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
  tft_write_reg(0x01); // Software Reset
  tft_Delay(1);

  ///!

  tft_write_reg(0x01); // Software Reset
  HAL_Delay(1);
  tft_write_reg(0xCB); // Power Control A
  tft_write_data(0x39);
  tft_write_data(0x2C);
  tft_write_data(0x00);
  tft_write_data(0x34);
  tft_write_data(0x02);
  HAL_Delay(1);
  tft_write_reg(0xCF); // Power Control B
  tft_write_data(0x00);
  tft_write_data(0xC1);
  tft_write_data(0x30);
  HAL_Delay(1);
  tft_write_reg(0xE8); // Driver timing control A
  tft_write_data(0x85);
  tft_write_data(0x00);
  tft_write_data(0x78);
  HAL_Delay(1);
  tft_write_reg(0xEA); // Driver timing control B
  tft_write_data(0x00);
  tft_write_data(0x00);
  HAL_Delay(1);
  tft_write_reg(0xED); // Power on Sequence control
  tft_write_data(0x64);
  tft_write_data(0x03);
  tft_write_data(0x12);
  tft_write_data(0x81);
  HAL_Delay(1);
  tft_write_reg(0xF7); // Pump ratio control
  tft_write_data(0x20);
  HAL_Delay(1);
  tft_write_reg(0xC0); // Power Control 1
  tft_write_data(0x10);
  HAL_Delay(1);
  tft_write_reg(0xC1); // Power Control 2
  tft_write_data(0x10);
  HAL_Delay(1);
  tft_write_reg(0xC5); // VCOM Control 1
  tft_write_data(0x3E);
  tft_write_data(0x28);
  HAL_Delay(1);
  tft_write_reg(0xC7); // VCOM Control 2
  tft_write_data(0x86);
  HAL_Delay(1);
  tft_write_reg(0x3A); // Pixel Format Set
  tft_write_data(0x55); // 16bit
  HAL_Delay(1);
  tft_write_reg(0xB1);
  tft_write_data(0x00);
  tft_write_data(0x18); // Частота кадров 79 Гц
  HAL_Delay(1);
  tft_write_reg(0xB6); // Display Function Control
  tft_write_data(0x08);
  tft_write_data(0x82);
  tft_write_data(0x27); // 320 строк
  HAL_Delay(1);
  tft_write_reg(0xF2); // Enable 3G
  tft_write_data(0x00); // не включаем
  HAL_Delay(1);
  tft_write_reg(0x26); // Gamma set
  tft_write_data(0x01); // Gamma Curve (G2.2) (Кривая цветовой гаммы)
  HAL_Delay(1);
  tft_write_reg(0xE0); // Positive Gamma Correction
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
  tft_write_reg(0xE1); // Negative Gamma Correction
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
  tft_write_reg(0x11); // Выйдем из спящего режим
  HAL_Delay(150);
  tft_SetRotation(0);
  HAL_Delay(1);
  tft_write_reg(0x29); // Включение дисплея
  tft_write_data(0x2C);

// /!
  tft_FontsIni();
  tft_SetRotation(0);
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
