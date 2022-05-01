#include "OTM8009A.h"
#include "tft_font.h"
#include "main.h"

void tft_Delay(volatile uint32_t delay);

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

__STATIC_INLINE void DelayMicro(uint32_t __IO micros)
{
  micros *=(SystemCoreClock/1000000)/5;
  while(micros--);
}

void tft_write_data(volatile uint16_t data)
{
  pin_high(RS_GPIO_Port,RS_Pin);
  tft_Delay(2);

  ADDR_DATA = (data);
}

void tft_write_reg(volatile uint16_t data)
{
  pin_low(RS_GPIO_Port,RS_Pin);
  tft_Delay(2);

  ADDR_CMD = (data);
}

uint32_t tft_ReadReg(uint8_t r)
{
  uint32_t id = 0;
  tft_write_reg(r);
  DelayMicro(50);
  for (uint8_t i = 0; i < 4; i++) { id <<= 8; id |= ADDR_DATA; }
  DelayMicro(150);
  return id;
}

#define OTM8009A_MADCTL 0x3600
#define OTM8009A_MADCTL_MY  0x80
#define OTM8009A_MADCTL_MX  0x40
#define OTM8009A_MADCTL_MV  0x20
#define OTM8009A_MADCTL_ML  0x10
#define OTM8009A_MADCTL_RGB 0x00
#define OTM8009A_MADCTL_BGR 0x08

void tft_SetRotation(uint8_t r)
{
  tft_write_reg(OTM8009A_MADCTL);
  switch(r)
  {
    case 0:
    tft_write_data(0x00);
    tft_W = 480;
    tft_H = 800;
    break;
    case 1:
    tft_write_data(OTM8009A_MADCTL_MX | OTM8009A_MADCTL_MV);
    tft_W = 800;
    tft_H = 480;
    break;
    case 2:
    tft_write_data(OTM8009A_MADCTL_MX | OTM8009A_MADCTL_MY);
    tft_W = 480;
    tft_H = 800;
    break;
    case 3:
    tft_write_data(OTM8009A_MADCTL_MY | OTM8009A_MADCTL_MV);
    tft_W = 800;
    tft_H = 480;
    break;
  }
}

void tft_Flood(uint16_t color,uint32_t len)
{
  uint16_t blocks;
  uint8_t i;
  tft_write_data(color);
  len--;
  blocks=(uint16_t)(len/64);// 64pixels/blocks
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
    }while(--i);
  }
  // Fill any remaining pixels (1 to 64);
  for(i=(uint8_t)len&63;i--;)
  {
    tft_write_data(color);
  }
}

void tft_FillScreen(uint16_t color)
{
  tft_address_set(0,0,tft_W-1,tft_H-1);
  tft_Flood(color,(long)tft_W*(long)tft_H);
}

void tft_FillRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_address_set(x1,y1,x2,y2);
  tft_Flood(color,(uint16_t)(x2-x1+1)*(uint16_t)(y2-y1+1));
}

void tft_DrawPixel(int64_t x, int64_t y, uint16_t color)
{
  if ((x<0)||(x>=tft_W)||(y<0)||(y>=tft_H)) return;
  tft_address_set(x,y,x,y);
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
      index=bufbmp[10];// адрес начала данных в файле
      index|=bufbmp[11]<<8;
      index|=bufbmp[12]<<16;
      index|=bufbmp[13]<<24;
      width=bufbmp[18];// ширина растра
      width|=bufbmp[19]<<8;
      width|=bufbmp[20]<<16;
      width|=bufbmp[21]<<24;
      height=bufbmp[22];// высота растра
      height|=bufbmp[23]<<8;
      height|=bufbmp[24]<<16;
      height|=bufbmp[25]<<24;
      bitpixel=bufbmp[28];// формат пикселя (бит/пиксель)
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
  HAL_Delay(10);
  pin_low(TFT_RST_GPIO_Port,TFT_RST_Pin);
  HAL_Delay(50);
  pin_high(TFT_RST_GPIO_Port,TFT_RST_Pin);
  HAL_Delay(10);
}

void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_write_reg(0x2A00);  tft_write_data(x1 >> 8);
  tft_write_reg(0x2A01);  tft_write_data(x1 & 0x10ff);
  tft_write_reg(0x2A02);  tft_write_data(x2 >> 8);
  tft_write_reg(0x2A03);  tft_write_data(x2 & 0x10ff);
  tft_write_reg(0x2B00);  tft_write_data(y1 >> 8);
  tft_write_reg(0x2B01);  tft_write_data(y1 & 0x10ff);
  tft_write_reg(0x2B02);  tft_write_data(y2 >> 8);
  tft_write_reg(0x2B03);  tft_write_data(y2 & 0x10ff);
  tft_write_reg(0x2C00);  tft_write_data(y2 & 0x1000);
}

void pre_OTM8009A_init(void)
{
  char str[11];
  tft_reset();
  // HAL_GPIO_WritePin(STM_LED1_GPIO_Port, STM_LED1_Pin, GPIO_PIN_RESET); // LED1
  // HAL_GPIO_WritePin(STM_LED2_GPIO_Port, STM_LED2_Pin, GPIO_PIN_RESET); // LED2
  HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET); // LIGHT
  tft_reset();
  
  HAL_Delay(100);
  // --
  dtt = tft_ReadReg(0xD3);
  LCD_Clear();
  LCD_SetPos(0,0);
  sprintf(str,"0x%08lX",(unsigned long)dtt);
  LCD_String(str);
  // --
  tft_write_reg(0x01);// oftware Reset
  DelayMicro(1);
}

void tft_ini(void)
{
  pre_OTM8009A_init();
  // ============ OTM8009A+HSD3.97 20140613 ===============================================// 
  tft_write_reg(0xff00); tft_write_data(0x80); // enable access command2
  tft_write_reg(0xff01); tft_write_data(0x09); // enable access command2
  tft_write_reg(0xff02); tft_write_data(0x01); // enable access command2
  tft_write_reg(0xff80); tft_write_data(0x80); // enable access command2
  tft_write_reg(0xff81); tft_write_data(0x09); // enable access command2
  tft_write_reg(0xff03); tft_write_data(0x01); // 
  tft_write_reg(0xc5b1); tft_write_data(0xA9); // power control

  tft_write_reg(0xc591); tft_write_data(0x0F); // power control
  tft_write_reg(0xc0B4); tft_write_data(0x50);

  // panel driving mode : column inversion

  // gamma
  tft_write_reg(0xE100); tft_write_data(0x00);
  tft_write_reg(0xE101); tft_write_data(0x09);
  tft_write_reg(0xE102); tft_write_data(0x0F);
  tft_write_reg(0xE103); tft_write_data(0x0E);
  tft_write_reg(0xE104); tft_write_data(0x07);
  tft_write_reg(0xE105); tft_write_data(0x10);
  tft_write_reg(0xE106); tft_write_data(0x0B);
  tft_write_reg(0xE107); tft_write_data(0x0A);
  tft_write_reg(0xE108); tft_write_data(0x04);
  tft_write_reg(0xE109); tft_write_data(0x07);
  tft_write_reg(0xE10A); tft_write_data(0x0B);
  tft_write_reg(0xE10B); tft_write_data(0x08);
  tft_write_reg(0xE10C); tft_write_data(0x0F);
  tft_write_reg(0xE10D); tft_write_data(0x10);
  tft_write_reg(0xE10E); tft_write_data(0x0A);
  tft_write_reg(0xE10F); tft_write_data(0x01);
  tft_write_reg(0xE200); tft_write_data(0x00);
  tft_write_reg(0xE201); tft_write_data(0x09);
  tft_write_reg(0xE202); tft_write_data(0x0F);
  tft_write_reg(0xE203); tft_write_data(0x0E);
  tft_write_reg(0xE204); tft_write_data(0x07);
  tft_write_reg(0xE205); tft_write_data(0x10);
  tft_write_reg(0xE206); tft_write_data(0x0B);
  tft_write_reg(0xE207); tft_write_data(0x0A);
  tft_write_reg(0xE208); tft_write_data(0x04);
  tft_write_reg(0xE209); tft_write_data(0x07);
  tft_write_reg(0xE20A); tft_write_data(0x0B);
  tft_write_reg(0xE20B); tft_write_data(0x08);
  tft_write_reg(0xE20C); tft_write_data(0x0F);
  tft_write_reg(0xE20D); tft_write_data(0x10);
  tft_write_reg(0xE20E); tft_write_data(0x0A);
  tft_write_reg(0xE20F); tft_write_data(0x01);
  tft_write_reg(0xD900); tft_write_data(0x4E); // vcom setting
  //
  tft_write_reg(0xc181); tft_write_data(0x66); // osc=65HZ
  //
  tft_write_reg(0xc1a1); tft_write_data(0x08);
  tft_write_reg(0xc592); tft_write_data(0x01); // power control
  tft_write_reg(0xc595); tft_write_data(0x34); // power control
  tft_write_reg(0xd800); tft_write_data(0x79); // GVDD / NGVDD setting
  tft_write_reg(0xd801); tft_write_data(0x79); // GVDD / NGVDD setting
  tft_write_reg(0xc594); tft_write_data(0x33); // power control
  tft_write_reg(0xc0a3); tft_write_data(0x1B); // panel timing setting
  tft_write_reg(0xc582); tft_write_data(0x83); // power control
  tft_write_reg(0xc481); tft_write_data(0x83); // source driver setting
  //
  tft_write_reg(0xc1a1); tft_write_data(0x0E);
  tft_write_reg(0xb3a6); tft_write_data(0x20);
  tft_write_reg(0xb3a7); tft_write_data(0x01);
  tft_write_reg(0xce80); tft_write_data(0x85); // GOA VST
  //
  tft_write_reg(0xce81); tft_write_data(0x01); // GOA VST
  tft_write_reg(0xce82); tft_write_data(0x00); // GOA VST
  tft_write_reg(0xce83); tft_write_data(0x84); // GOA VST
  tft_write_reg(0xce84); tft_write_data(0x01); // GOA VST
  tft_write_reg(0xce85); tft_write_data(0x00); // GOA VST
  //
  tft_write_reg(0xcea0); tft_write_data(0x18); // GOA CLK
  tft_write_reg(0xcea1); tft_write_data(0x04); // GOA CLK
  tft_write_reg(0xcea2); tft_write_data(0x03); // GOA CLK
  tft_write_reg(0xcea3); tft_write_data(0x39); // GOA CLK
  tft_write_reg(0xcea4); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcea5); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcea6); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcea7); tft_write_data(0x18); // GOA CLK
  tft_write_reg(0xcea8); tft_write_data(0x03); // GOA CLK
  tft_write_reg(0xcea9); tft_write_data(0x03); // GOA CLK
  tft_write_reg(0xceaa); tft_write_data(0x3a); // GOA CLK
  tft_write_reg(0xceab); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xceac); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcead); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xceb0); tft_write_data(0x18); // GOA CLK
  tft_write_reg(0xceb1); tft_write_data(0x02); // GOA CLK
  tft_write_reg(0xceb2); tft_write_data(0x03); // GOA CLK
  tft_write_reg(0xceb3); tft_write_data(0x3b); // GOA CLK
  tft_write_reg(0xceb4); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xceb5); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xceb6); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xceb7); tft_write_data(0x18); // GOA CLK
  tft_write_reg(0xceb8); tft_write_data(0x01); // GOA CLK
  tft_write_reg(0xceb9); tft_write_data(0x03); // GOA CLK
  tft_write_reg(0xceba); tft_write_data(0x3c); // GOA CLK
  tft_write_reg(0xcebb); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcebc); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcebd); tft_write_data(0x00); // GOA CLK
  tft_write_reg(0xcfc0); tft_write_data(0x01); // GOA ECLK
  tft_write_reg(0xcfc1); tft_write_data(0x01); // GOA ECLK
  tft_write_reg(0xcfc2); tft_write_data(0x20); // GOA ECLK
  //
  tft_write_reg(0xcfc3); tft_write_data(0x20); // GOA ECLK
  tft_write_reg(0xcfc4); tft_write_data(0x00); // GOA ECLK
  tft_write_reg(0xcfc5); tft_write_data(0x00); // GOA ECLK
  //
  tft_write_reg(0xcfc6); tft_write_data(0x01); // GOA other options
  //
  tft_write_reg(0xcfc7); tft_write_data(0x00);
  //
  // GOA signal toggle option setting
  //
  tft_write_reg(0xcfc8); tft_write_data(0x00); // GOA signal toggle option setting
  tft_write_reg(0xcfc9); tft_write_data(0x00);
  //
  // GOA signal toggle option setting
  //
  tft_write_reg(0xcfd0); tft_write_data(0x00);
  tft_write_reg(0xcb80); tft_write_data(0x00);
  tft_write_reg(0xcb81); tft_write_data(0x00);
  tft_write_reg(0xcb82); tft_write_data(0x00);
  tft_write_reg(0xcb83); tft_write_data(0x00);
  tft_write_reg(0xcb84); tft_write_data(0x00);
  tft_write_reg(0xcb85); tft_write_data(0x00);
  tft_write_reg(0xcb86); tft_write_data(0x00);
  tft_write_reg(0xcb87); tft_write_data(0x00);
  tft_write_reg(0xcb88); tft_write_data(0x00);
  tft_write_reg(0xcb89); tft_write_data(0x00);
  tft_write_reg(0xcb90); tft_write_data(0x00);
  tft_write_reg(0xcb91); tft_write_data(0x00);
  tft_write_reg(0xcb92); tft_write_data(0x00);
  tft_write_reg(0xcb93); tft_write_data(0x00);
  tft_write_reg(0xcb94); tft_write_data(0x00);
  tft_write_reg(0xcb95); tft_write_data(0x00);
  tft_write_reg(0xcb96); tft_write_data(0x00);
  tft_write_reg(0xcb97); tft_write_data(0x00);
  tft_write_reg(0xcb98); tft_write_data(0x00);
  tft_write_reg(0xcb99); tft_write_data(0x00);
  tft_write_reg(0xcb9a); tft_write_data(0x00);
  tft_write_reg(0xcb9b); tft_write_data(0x00);
  tft_write_reg(0xcb9c); tft_write_data(0x00);
  tft_write_reg(0xcb9d); tft_write_data(0x00);
  tft_write_reg(0xcb9e); tft_write_data(0x00);
  tft_write_reg(0xcba0); tft_write_data(0x00);
  tft_write_reg(0xcba1); tft_write_data(0x00);
  tft_write_reg(0xcba2); tft_write_data(0x00);
  tft_write_reg(0xcba3); tft_write_data(0x00);
  tft_write_reg(0xcba4); tft_write_data(0x00);
  tft_write_reg(0xcba5); tft_write_data(0x00);
  tft_write_reg(0xcba6); tft_write_data(0x00);
  tft_write_reg(0xcba7); tft_write_data(0x00);
  tft_write_reg(0xcba8); tft_write_data(0x00);
  tft_write_reg(0xcba9); tft_write_data(0x00);
  tft_write_reg(0xcbaa); tft_write_data(0x00);
  tft_write_reg(0xcbab); tft_write_data(0x00);
  tft_write_reg(0xcbac); tft_write_data(0x00);
  tft_write_reg(0xcbad); tft_write_data(0x00);
  tft_write_reg(0xcbae); tft_write_data(0x00);
  tft_write_reg(0xcbb0); tft_write_data(0x00);
  tft_write_reg(0xcbb1); tft_write_data(0x00);
  tft_write_reg(0xcbb2); tft_write_data(0x00);
  tft_write_reg(0xcbb3); tft_write_data(0x00);
  tft_write_reg(0xcbb4); tft_write_data(0x00);
  tft_write_reg(0xcbb5); tft_write_data(0x00);
  tft_write_reg(0xcbb6); tft_write_data(0x00);
  tft_write_reg(0xcbb7); tft_write_data(0x00);
  tft_write_reg(0xcbb8); tft_write_data(0x00);
  tft_write_reg(0xcbb9); tft_write_data(0x00);
  tft_write_reg(0xcbc0); tft_write_data(0x00);
  tft_write_reg(0xcbc1); tft_write_data(0x04);
  tft_write_reg(0xcbc2); tft_write_data(0x04);
  tft_write_reg(0xcbc3); tft_write_data(0x04);
  tft_write_reg(0xcbc4); tft_write_data(0x04);
  tft_write_reg(0xcbc5); tft_write_data(0x04);
  tft_write_reg(0xcbc6); tft_write_data(0x00);
  tft_write_reg(0xcbc7); tft_write_data(0x00);
  tft_write_reg(0xcbc8); tft_write_data(0x00);
  tft_write_reg(0xcbc9); tft_write_data(0x00);
  tft_write_reg(0xcbca); tft_write_data(0x00);
  tft_write_reg(0xcbcb); tft_write_data(0x00);
  tft_write_reg(0xcbcc); tft_write_data(0x00);
  tft_write_reg(0xcbcd); tft_write_data(0x00);
  tft_write_reg(0xcbce); tft_write_data(0x00);
  tft_write_reg(0xcbd0); tft_write_data(0x00);
  tft_write_reg(0xcbd1); tft_write_data(0x00);
  tft_write_reg(0xcbd2); tft_write_data(0x00);
  tft_write_reg(0xcbd3); tft_write_data(0x00);
  tft_write_reg(0xcbd4); tft_write_data(0x00);
  tft_write_reg(0xcbd5); tft_write_data(0x00);
  tft_write_reg(0xcbd6); tft_write_data(0x04);
  tft_write_reg(0xcbd7); tft_write_data(0x04);
  tft_write_reg(0xcbd8); tft_write_data(0x04);
  tft_write_reg(0xcbd9); tft_write_data(0x04);
  tft_write_reg(0xcbda); tft_write_data(0x04);
  tft_write_reg(0xcbdb); tft_write_data(0x00);
  tft_write_reg(0xcbdc); tft_write_data(0x00);
  tft_write_reg(0xcbdd); tft_write_data(0x00);
  tft_write_reg(0xcbde); tft_write_data(0x00);
  tft_write_reg(0xcbe0); tft_write_data(0x00);
  tft_write_reg(0xcbe1); tft_write_data(0x00);
  tft_write_reg(0xcbe2); tft_write_data(0x00);
  tft_write_reg(0xcbe3); tft_write_data(0x00);
  tft_write_reg(0xcbe4); tft_write_data(0x00);
  tft_write_reg(0xcbe5); tft_write_data(0x00);
  tft_write_reg(0xcbe6); tft_write_data(0x00);
  tft_write_reg(0xcbe7); tft_write_data(0x00);
  tft_write_reg(0xcbe8); tft_write_data(0x00);
  tft_write_reg(0xcbe9); tft_write_data(0x00);
  tft_write_reg(0xcbf0); tft_write_data(0xFF);
  tft_write_reg(0xcbf1); tft_write_data(0xFF);
  tft_write_reg(0xcbf2); tft_write_data(0xFF);
  tft_write_reg(0xcbf3); tft_write_data(0xFF);
  tft_write_reg(0xcbf4); tft_write_data(0xFF);
  tft_write_reg(0xcbf5); tft_write_data(0xFF);
  tft_write_reg(0xcbf6); tft_write_data(0xFF);
  tft_write_reg(0xcbf7); tft_write_data(0xFF);
  tft_write_reg(0xcbf8); tft_write_data(0xFF);
  tft_write_reg(0xcbf9); tft_write_data(0xFF);
  tft_write_reg(0xcc80); tft_write_data(0x00);
  tft_write_reg(0xcc81); tft_write_data(0x26);
  tft_write_reg(0xcc82); tft_write_data(0x09);
  tft_write_reg(0xcc83); tft_write_data(0x0B);
  tft_write_reg(0xcc84); tft_write_data(0x01);
  tft_write_reg(0xcc85); tft_write_data(0x25);
  tft_write_reg(0xcc86); tft_write_data(0x00);
  tft_write_reg(0xcc87); tft_write_data(0x00);
  tft_write_reg(0xcc88); tft_write_data(0x00);
  tft_write_reg(0xcc89); tft_write_data(0x00);
  tft_write_reg(0xcc90); tft_write_data(0x00);
  tft_write_reg(0xcc91); tft_write_data(0x00);
  tft_write_reg(0xcc92); tft_write_data(0x00);
  tft_write_reg(0xcc93); tft_write_data(0x00);
  tft_write_reg(0xcc94); tft_write_data(0x00);
  tft_write_reg(0xcc95); tft_write_data(0x00);
  tft_write_reg(0xcc96); tft_write_data(0x00);
  tft_write_reg(0xcc97); tft_write_data(0x00);
  tft_write_reg(0xcc98); tft_write_data(0x00);
  tft_write_reg(0xcc99); tft_write_data(0x00);
  tft_write_reg(0xcc9a); tft_write_data(0x00);
  tft_write_reg(0xcc9b); tft_write_data(0x26);
  tft_write_reg(0xcc9c); tft_write_data(0x0A);
  tft_write_reg(0xcc9d); tft_write_data(0x0C);
  tft_write_reg(0xcc9e); tft_write_data(0x02);
  tft_write_reg(0xcca0); tft_write_data(0x25);
  tft_write_reg(0xcca1); tft_write_data(0x00);
  tft_write_reg(0xcca2); tft_write_data(0x00);
  tft_write_reg(0xcca3); tft_write_data(0x00);
  tft_write_reg(0xcca4); tft_write_data(0x00);
  tft_write_reg(0xcca5); tft_write_data(0x00);
  tft_write_reg(0xcca6); tft_write_data(0x00);
  tft_write_reg(0xcca7); tft_write_data(0x00);
  tft_write_reg(0xcca8); tft_write_data(0x00);
  tft_write_reg(0xcca9); tft_write_data(0x00);
  tft_write_reg(0xccaa); tft_write_data(0x00);
  tft_write_reg(0xccab); tft_write_data(0x00);
  tft_write_reg(0xccac); tft_write_data(0x00);
  tft_write_reg(0xccad); tft_write_data(0x00);
  tft_write_reg(0xccae); tft_write_data(0x00);
  tft_write_reg(0xccb0); tft_write_data(0x00);
  tft_write_reg(0xccb1); tft_write_data(0x25);
  tft_write_reg(0xccb2); tft_write_data(0x0C);
  tft_write_reg(0xccb3); tft_write_data(0x0A);
  tft_write_reg(0xccb4); tft_write_data(0x02);
  tft_write_reg(0xccb5); tft_write_data(0x26);
  tft_write_reg(0xccb6); tft_write_data(0x00);
  tft_write_reg(0xccb7); tft_write_data(0x00);
  tft_write_reg(0xccb8); tft_write_data(0x00);
  tft_write_reg(0xccb9); tft_write_data(0x00);
  tft_write_reg(0xccc0); tft_write_data(0x00);
  tft_write_reg(0xccc1); tft_write_data(0x00);
  tft_write_reg(0xccc2); tft_write_data(0x00);
  tft_write_reg(0xccc3); tft_write_data(0x00);
  tft_write_reg(0xccc4); tft_write_data(0x00);
  tft_write_reg(0xccc5); tft_write_data(0x00);
  tft_write_reg(0xccc6); tft_write_data(0x00);
  tft_write_reg(0xccc7); tft_write_data(0x00);
  tft_write_reg(0xccc8); tft_write_data(0x00);
  tft_write_reg(0xccc9); tft_write_data(0x00);
  tft_write_reg(0xccca); tft_write_data(0x00);
  tft_write_reg(0xcccb); tft_write_data(0x25);
  tft_write_reg(0xcccc); tft_write_data(0x0B);
  tft_write_reg(0xcccd); tft_write_data(0x09);
  tft_write_reg(0xccce); tft_write_data(0x01);
  tft_write_reg(0xccd0); tft_write_data(0x26);
  tft_write_reg(0xccd1); tft_write_data(0x00);
  tft_write_reg(0xccd2); tft_write_data(0x00);
  tft_write_reg(0xccd3); tft_write_data(0x00);
  tft_write_reg(0xccd4); tft_write_data(0x00);
  tft_write_reg(0xccd5); tft_write_data(0x00);
  tft_write_reg(0xccd6); tft_write_data(0x00);
  tft_write_reg(0xccd7); tft_write_data(0x00);
  tft_write_reg(0xccd8); tft_write_data(0x00);
  tft_write_reg(0xccd9); tft_write_data(0x00);
  tft_write_reg(0xccda); tft_write_data(0x00);
  tft_write_reg(0xccdb); tft_write_data(0x00);
  tft_write_reg(0xccdc); tft_write_data(0x00);
  tft_write_reg(0xccdd); tft_write_data(0x00);
  tft_write_reg(0xccde); tft_write_data(0x00);
  tft_write_reg(0x3A00); tft_write_data(0x55);
  //
  tft_write_reg(0x1100);
  HAL_Delay(100);
  //
  tft_write_reg(0x2900);
  HAL_Delay(50);
  //
  tft_write_reg(0x2C00);
  tft_write_reg(0x2A00); tft_write_data(0x00);
  tft_write_reg(0x2A01); tft_write_data(0x00);
  tft_write_reg(0x2A02); tft_write_data(0x01);
  tft_write_reg(0x2A03); tft_write_data(0xe0);
  tft_write_reg(0x2B00); tft_write_data(0x00);
  tft_write_reg(0x2B01); tft_write_data(0x00);
  tft_write_reg(0x2B02); tft_write_data(0x03);
  tft_write_reg(0x2B03); tft_write_data(0x20);
  //  
  // tft_SetRotation(0);
  tft_SetRotation(0);
  //
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
