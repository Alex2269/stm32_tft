#include "ssd1963.h"
#include "tft_font.h"

#define MIRROR_HORIZONTAL 1
#define MIRROR_VERTICAL 0
#define LSHIFT 1

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
  pin_high(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_Delay(5);
  ADDR_DATA = (data);
}

void tft_write_reg(volatile uint16_t data)
{
  pin_low(DSP_RS_GPIO_Port,DSP_RS_Pin);
  tft_Delay(5);
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
  if(r==0)
  {
    tft_write_data(0x40|0x08);
    tft_W = 480;
    tft_H = 800;
  }
  if(r==1)
  {
    tft_write_data(0x20|0x08);
    tft_W = 800;
    tft_H = 480;
  }
  if(r==2)
  {
    tft_write_data(0x80|0x08);
    tft_W = 480;
    tft_H = 800;
  }
  if(r==3)
  {
    tft_write_data(0x40|0x80|0x20|0x08);
    tft_W = 800;
    tft_H = 480;
  }
}

void tft_Flood(uint32_t color,uint32_t len)
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

void tft_FillScreen(uint32_t color)
{
  tft_SetAddrWindow(0,0,tft_W-1,tft_H-1);
  tft_Flood(color,(uint16_t)tft_W*(uint16_t)tft_H);
}

void tft_FillRect(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_SetAddrWindow(x1,y1,x2,y2);
  tft_Flood(color,(uint16_t)(x2-x1+1)*(uint16_t)(y2-y1+1));
}

void tft_DrawPixel(int64_t x, int64_t y, uint32_t color)
{
  if((x<0)||(y<0)||(x>=tft_W)||(y>=tft_H)) return;
  tft_SetAddrWindow(x,y,x,y);
  tft_write_reg(0x2C);
  tft_write_data(color);
}

void tft_DrawLine(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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

void tft_DrawRect(uint32_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tft_DrawLine(color,x1,y1,x2,y1);
  tft_DrawLine(color,x2,y1,x2,y2);
  tft_DrawLine(color,x1,y1,x1,y2);
  tft_DrawLine(color,x1,y2,x2,y2);
}

void tft_DrawCircle(uint16_t x0, uint16_t y0, int64_t r, uint32_t color)
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
  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
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
  pin_high(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay (100);
  pin_low(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay (120);
  pin_high(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay(150);

  tft_write_reg(0x01);
  for(uint8_t i=0;i<3;i++) tft_write_data(0xFF);
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
  // HAL_GPIO_WritePin(STM_LED1_GPIO_Port, STM_LED1_Pin, GPIO_PIN_RESET); // LED1
  // HAL_GPIO_WritePin(STM_LED2_GPIO_Port, STM_LED2_Pin, GPIO_PIN_RESET); // LED2
  HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET); // LIGHT
  HAL_Delay(5);

  #define HDP 799 // HDP 0x31f
  #define HT 928 // HT 0x3a0
  #define HPS 46 // HPS 0x02e
  #define LPS 15 // LPS 0x00f
  #define HPW 48 // HPW 0x30
  #define VDP 479 // VDP 0x1df
  #define VT 525 // VT 0x20d
  #define VPS 16 // VPS 0x016
  #define FPS 8 // FPS 0x008
  #define VPW 16 // VPW 0x010

  tft_reset();

  tft_write_reg(0xE2);// et frequency
  tft_write_data(0x1D); // presceller(M=29)
  tft_write_data(0x02); // ultiplier(N=2)
  tft_write_data(0xFF); // n-off multiplier and presceller
  // . Turn on the PLL
  tft_write_reg(0xE0);
  tft_write_data(0x01);
  HAL_Delay(120); // Wait for 100us to let the PLL stable and read the PLL lock status bit.
  tft_write_reg(0xE0);
  // EAD COMMAND “0xE4); (Bit 2 = 1 if PLL locked)
  tft_write_data(0x03); // 5. Switch the clock source to PLL
  HAL_Delay(120);
  tft_write_reg(0x01); // . Software Reset
  HAL_Delay(120);
  /*************
  Dot clock Freq = PLL Freq x (LCDC_FPR + 1) / 2^20
  For example, 22MHz = 100MHz * (LCDC_FPR+1) / 2^20
  LCDC_FPR = 230685 = 0x3851D
  ********************/
  tft_write_reg(0xE6); // 7. Configure the dot clock frequency
  tft_write_data(0x03);
  tft_write_data(0x85);
  tft_write_data(0x1D);
  // . Configure the LCD panel
  // . Set the panel size to 480 x 800 and polarity of LSHIFT, LLINE and LFRAME to active low
  if (LSHIFT)
  {
    tft_write_reg(0xB0);
    tft_write_data(0x0C); /* 0x08 0x0C 0xAE(5') */
  }
  if (!LSHIFT)
  {
    tft_write_reg(0xB0);
    tft_write_data(0xAE); // 8bit panel, disable dithering, LSHIFT: Data latch in rising edge, LLINE and LFRAME: active low
  }
  tft_write_data(0x20); /* 0x00 0x80 0x20(5') */ // TFT type
  tft_write_data(0x03); // Horizontal Width: 480 - 1 = 0x031F
  tft_write_data(0x1F);
  tft_write_data(0x01); // Vertical Width : 800 -1 = 0x01DF
  tft_write_data(0xDF);
  tft_write_data(0x2D); /* 0x00 0x2d */ // 000 = режим RGB
  // . Set the horizontal period
  tft_write_reg(0xB4); // Horizontal Display Period
  tft_write_data(0x03); // HT: horizontal total period (display + non-display) – 1 = 520-1 = 519 =0x0207
  tft_write_data(0xA0);
  tft_write_data(0x00); // HPS: Horizontal Sync Pulse Start Position = Horizontal Pulse Width + Horizontal Back Porch = 16 = 0x10
  tft_write_data(0x2E);
  tft_write_data(0x30); // HPW: Horizontal Sync Pulse Width - 1=8-1=7
  tft_write_data(0x00); // LPS: Horizontal Display Period Start Position = 0x0000
  tft_write_data(0x0F);
  tft_write_data(0x00); // LPSPP: Horizontal Sync Pulse Subpixel Start Position(for serial TFT interface). Dummy value for TFT interface.
  // . Set the vertical period
  tft_write_reg(0xB6); // Vertical Display Period
  tft_write_data(0x02); // VT: Vertical Total (display + non-display) Period – 1 =647=0x287
  tft_write_data(0x0D);
  tft_write_data(0x00); // VPS: Vertical Sync Pulse Start Position = Vertical Pulse Width + Vertical Back Porch = 2+2=4
  tft_write_data(0x10);
  tft_write_data(0x10); // PW: Vertical Sync Pulse Width – 1 =1
  tft_write_data(0x08); // PS: Vertical Display Period Start Position = 0
  tft_write_data(0x00);
  // . Set the back light control PWM clock frequency
  // WM signal frequency = PLL clock / (256 * (PWMF[7:0] + 1)) / 256
  tft_write_reg(0xBE); // PWM configuration
  tft_write_data(0x08); // set PWM signal frequency to 170Hz when PLL frequency is 100MHz
  tft_write_data(0xFF); // PWM duty cycle (50%)
  tft_write_data(0x01); // 0x09 = enable DBC, 0x01 = disable DBC // n
  HAL_Delay(5);
  if (MIRROR_VERTICAL)
  {
    tft_write_reg(0x0036);
    tft_write_data(0x0001);
  }
  if (MIRROR_HORIZONTAL)
  {
    tft_write_reg(0x0036); // otation horizontal
    tft_write_data(0x02);
  }
  tft_write_reg(0x00F0); // Set pixel data interface
  tft_write_data(0x03); // 16 bit interface (565)
  HAL_Delay(120);
  tft_write_reg(0x0029); // Show the image on the display device
  tft_write_data(0x2C);
  // !
  tft_color_fonts();
}

void tft_clear(uint32_t color)
{
  uint16_t i,j;
  tft_address_set(0,0,tft_W-1,tft_H-1);
  for(i=0;i<tft_W;i++)
  {
    for (j=0;j<tft_H;j++)
    {
      pin_high(DSP_RS_GPIO_Port,DSP_RS_Pin);
      // tft_write_data(color>>8);
      // tft_write_data(color&0x00ff);
      tft_write_data(color);
      // pin_low(DSP_RS_GPIO_Port,DSP_RS_Pin);
    }
  }
}

void tft_draw_point(uint16_t x,uint16_t y,uint32_t color)
{
  tft_address_set(x,y,x,y);
  tft_write_data(color);
}

void tft_draw_point_big(uint16_t x,uint16_t y,uint32_t color)
{
  tft_fill(x-1,y-1,x+1,y+1,color);
}

void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint32_t color)
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

