
#include "main.h"
#include "st7775.h"
#include "tft_font.h"
#define delay_ms(ms) LL_mDelay(ms)

uint16_t BACK_COLOR,POINT_COLOR;


uint16_t tft_W = 176;
uint16_t tft_H = 220;
uint32_t dtt=0;
uint32_t TextColor;
uint32_t BackColor;
uint8_t *pchar, *str2;

typedef struct
{
  uint16_t TextColor;
  uint16_t BackColor;
} LCD_DrawPropTypeDef;

LCD_DrawPropTypeDef lcdprop;

// #define MODE_INPUT        (0U) /*!< Select input mode */
// #define MODE_OUTPUT       0x1UL  /*!< Select output mode */
// #define MODE_ALTERNATE    0x2UL  /*!< Select alternate function mode */
// #define MODE_ANALOG       0x3UL    /*!< Select analog mode */

// // for cortex m3
// void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
// {
//   register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin >> 24)));
//   MODIFY_REG(*pReg, ((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U)), (Mode << (POSITION_VAL(Pin) * 4U)));
// }

// // for cortex m4
// static inline
// void SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
// {
//   MODIFY_REG(GPIOx->MODER, (3U << (POSITION_VAL(Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
// }

static inline
void SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, PinMask);
}

static inline
void ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, (PinMask << 16));
}

static inline
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint16_t state)
{
  if(state)
  {
      SetOutputPin(GPIOx, PinMask);
  }
  else
  {
      ResetOutputPin(GPIOx, PinMask);
  }
}

static inline
void SetCursorPos(uint16_t x, uint16_t y)
{
  WriteRegister(0x20); WriteData(y);
  WriteRegister(0x21); WriteData(x);
}

uint8_t ScrollIndex = 0;

void WriteRegister(uint8_t IndexRegister)
{
  GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);
  GPIO_WritePin(RS_GPIO_Port, RS_Pin, 0);

  GPIOA->ODR = IndexRegister;
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);
  GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);
}

void WriteData(uint16_t Data)
{
  GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);
  GPIO_WritePin(RS_GPIO_Port, RS_Pin, 1);

  GPIOA->ODR = Data >> 8;
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);
  GPIOA->ODR = Data;
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);

  GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);
}

void DrawHLine(uint8_t X, uint8_t Y, uint8_t Width, uint16_t Color)
{
  WriteRegister(0x37);
  WriteData(Y);
  WriteRegister(0x36);
  WriteData(Y);
  WriteRegister(0x39);
  WriteData(X);
  WriteRegister(0x38);
  WriteData(X + Width);
  
  SetCursorPos(X, Y);
  
  WriteRegister(0x22); //DRAM write data
  
  for(uint8_t i = 0 ; i <= Width ; ++i)
  {
    WriteColor(Color);
  }
}

void DrawShape8x8(const uint8_t X, uint8_t Y, const uint8_t Scale, const uint8_t* Shape, const uint8_t Palette, const bool FlipVertical, const bool FlipHorizontal)
{
  WriteRegister(0x37);
  WriteData(Y);
  WriteRegister(0x36);
  WriteData(Y + ((8 * Scale) - 1));
  WriteRegister(0x39);
  WriteData(X);
  WriteRegister(0x38);
  WriteData(X + ((8 * Scale) - 1));
  
  int8_t IncDec = 1;
  if(FlipVertical)
  {
    IncDec = -1;
    Y += ((8 * Scale) - 1);
  }
  
  SetCursorPos(X, Y);

  uint8_t IndexData1;
  uint8_t IndexData2;
  uint8_t IndexData3;
  uint8_t ActualIndex;
  uint16_t Color;
  uint8_t CurrentX = X;
  uint8_t RepeatCount = 0;

  for(uint8_t i = 0 ; i < 8 ; ++i)
  {
    ActualIndex = (3 * i); //Multiply by 3 because every loop uses 3 indexes
    IndexData1 = Shape[ActualIndex];
    IndexData2 = Shape[ActualIndex + 1];
    IndexData3 = Shape[ActualIndex + 2];
    
    for(uint8_t j = 0 ; j < Scale ; ++j)
    {
      //Actual index taken from 3 bits
      if(FlipHorizontal)
        ActualIndex = ((IndexData3 >> 5) & 0b00000111);
      else
        ActualIndex = IndexData1 & 0b00000111;
      if(ActualIndex != 7) //Color is not transparent
      {
        Color = COLOR_OLIVE;//
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData3 >> 2) & 0b00000111);
      else
        ActualIndex = ((IndexData1 >> 3) & 0b00000111);
      if(ActualIndex != 7)
      {
        Color = COLOR_CYAN;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData2 >> 7) & 0b00000001) | ((IndexData3 << 1) & 0b00000110);
      else
        ActualIndex = ((IndexData1 >> 6) & 0b00000011) | ((IndexData2 << 2) & 0b00000100);
      if(ActualIndex != 7)
      {
        Color = COLOR_BLUE;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData2 >> 4) & 0b00000111);
      else
        ActualIndex = ((IndexData2 >> 1) & 0b00000111);
      if(ActualIndex != 7)
      {
        Color = COLOR_GREEN;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData2 >> 1) & 0b00000111);
      else
        ActualIndex = ((IndexData2 >> 4) & 0b00000111);
      if(ActualIndex != 7)
      {
        Color = COLOR_MAGENTA;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData1 >> 6) & 0b00000011) | ((IndexData2 << 2) & 0b00000100);
      else
        ActualIndex = ((IndexData2 >> 7) & 0b00000001) | ((IndexData3 << 1) & 0b00000110);
      if(ActualIndex != 7)
      {
        Color = COLOR_GREENYELLOW;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      CurrentX += Scale;
      if(FlipHorizontal)
        ActualIndex = ((IndexData1 >> 3) & 0b00000111);
      else
        ActualIndex = ((IndexData3 >> 2) & 0b00000111);
      if(ActualIndex != 7)
      {
        Color = COLOR_LIGHTGREY;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }
      else
      {
        SetCursorPos(CurrentX + Scale, Y);
      }
      
      if(FlipHorizontal)
        ActualIndex = IndexData1 & 0b00000111;
      else
        ActualIndex = ((IndexData3 >> 5) & 0b00000111);
      if(ActualIndex != 7)
      {
        Color = COLOR_DARKGREY;
        for(; RepeatCount < Scale ; ++RepeatCount)
        {
          WriteRegister(0x22);
          WriteColor(Color);
        }
        RepeatCount = 0;
      }

      CurrentX = X;
      Y += IncDec;
      
      SetCursorPos(X, Y);
    }
  }
}

void DrawShape16x16(const uint8_t X, uint8_t Y, const uint8_t Scale, const uint8_t* ShapeTopLeft, const uint8_t* ShapeTopRight, const uint8_t* ShapeBottomLeft, const uint8_t* ShapeBottomRight, const uint8_t Palette)
{
  DrawShape8x8(X, Y, Scale, ShapeTopLeft, Palette, false, false);
  DrawShape8x8(X + (8 * Scale), Y, Scale, ShapeTopRight, Palette, false, false);
  DrawShape8x8(X, Y + (8 * Scale), Scale, ShapeBottomLeft, Palette, true, false);
  DrawShape8x8(X + (8 * Scale), Y + (8 * Scale), Scale, ShapeBottomRight, Palette, true, false);
}

void ClearDisplay()
{
  WriteRegister(0x20); WriteData(0x0000);
  WriteRegister(0x21); WriteData(0x0000);
  WriteRegister(0x22);
  for(uint8_t i = 0 ; i < 220u ; ++i)
  {
    for(uint8_t j = 0 ; j < 176u ; ++j)
    {
      WriteData(0x0000);
    }
  }
}

void DrawArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint16_t Color)
{
  WriteRegister(0x37);
  WriteData(Y);
  WriteRegister(0x36);
  WriteData(Y + Height - 1);
  WriteRegister(0x39);
  WriteData(X);
  WriteRegister(0x38);
  WriteData(X + Width - 1);
  
  SetCursorPos(X, Y);

  WriteRegister(0x22);
  for(uint8_t i = 0 ; i < Width ; ++i)
  {
    for(uint8_t j = 0 ; j < Height ; ++j)
    {
      WriteColor(Color);
    }
  }
}

void ScrollLeft()
{
  if(++ScrollIndex == 219)
    ScrollIndex = 0;
  WriteRegister(0x33);
  WriteData(ScrollIndex);
}

void ScrollRight()
{
  if(--ScrollIndex == 255u)
    ScrollIndex = 219u;
  WriteRegister(0x33);
  WriteData(ScrollIndex);
}

void InitDisplay()
{
  GPIO_WritePin(RD_GPIO_Port, RD_Pin, 0);  //Reset screen
  GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0);  //Reset screen
  delay_ms(25);
  GPIO_WritePin(RD_GPIO_Port, RD_Pin, 1);  //Reset screen
  GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1);  //Reset screen
  delay_ms(10);
  //Backlight on
  GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
  delay_ms(1);

  WriteRegister(0x01);
  WriteData(0x001c); //Sets number of lines used (220 lines)
  WriteRegister(0x02);
  WriteData(0x0000); //Column or dot inversion (column)
  WriteRegister(0x03);
  WriteData(0x1038); //RGB, screen orientation
  WriteRegister(0x08);
  WriteData(0x0808); //Front and back porch period (8 lines each)
  WriteRegister(0x0B);
  WriteData(0x0000); //Internal clock synchronization (0 clocks)
  WriteRegister(0x0C);
  WriteData(0x0000); //RGB mode (16 bit)
  WriteRegister(0x0F);
  WriteData(0x0001); //Oscillator (on)
  delay_ms(10); //Wait at least 10ms for ocillator to stabilize
  
  WriteRegister(0x10);
  WriteData(0x0000); //Power (no sleep, no standby)
  WriteRegister(0x11);
  WriteData(0x1000); //Booster sequence circuit (on)

  WriteRegister(0xB0);
  WriteData(0x1411); //Power control
  WriteRegister(0xB1);
  WriteData(0x0202);
  WriteRegister(0xB2);
  WriteData(0x0313);
  
  WriteRegister(0x30);
  WriteData(0x0000); //Gate line start
  WriteRegister(0x31);
  WriteData(0x00DB); //Vertical scroll end (line 219)
  WriteRegister(0x32);
  WriteData(0x0000); //Verticall scroll start (line 0)
  WriteRegister(0x33);
  WriteData(0x0000); //Number of scrolling lines (disable scrolling)
  
  WriteRegister(0x34);
  WriteData(0x00DB); //Partial screen position end (219 lines)
  WriteRegister(0x35);
  WriteData(0x0000); //Partial screen position start
  
  WriteRegister(0x36);
  WriteData(0x00AF); //Horizontal end (175)
  WriteRegister(0x37);
  WriteData(0x0000); //Horizontal start (0)
  WriteRegister(0x38);
  WriteData(0x00DB); //Vertical start (219)
  WriteRegister(0x39);
  WriteData(0x0000); //Vertical end (0)
  
  //Gamma control
  /*WriteRegister(0xFF);
  WriteData(0x03);
  WriteRegister(0x50);
  WriteData(0x0000);
  WriteRegister(0x51);
  WriteData(0x0300);
  WriteRegister(0x52);
  WriteData(0x0103);
  WriteRegister(0x53);
  WriteData(0x2011);
  WriteRegister(0x54);
  WriteData(0x0703);
  WriteRegister(0x55);
  WriteData(0x0000);
  WriteRegister(0x56);
  WriteData(0x0400);
  WriteRegister(0x57);
  WriteData(0x0107);
  WriteRegister(0x58);
  WriteData(0x2011);
  WriteRegister(0x59);
  WriteData(0x0703);*/
  
  WriteRegister(0x20);
  WriteData(0x0000);
  WriteRegister(0x21);
  WriteData(0x0000);

  WriteRegister(0x07);
  WriteData(0b00010111); //Frame tearing (allowed), Grayscale (off), Display (on)

  delay_ms(10);

  ClearDisplay();

  ScrollIndex = 0;
}

void fillCircle(int x, int y, int radius, uint16_t Color)
{
  for(int y1=-radius; y1<=0; y1++)
  for(int x1=-radius; x1<=0; x1++)
  if(x1*x1+y1*y1 <= radius*radius)
  {
    DrawHLine(x+x1, y+y1, 2*(-x1), Color);
    DrawHLine(x+x1, y-y1, 2*(-x1), Color);
    break;
  }
}

void setWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  WriteRegister(0x37); WriteData(y1);
  WriteRegister(0x36); WriteData(y2);
  WriteRegister(0x39); WriteData(x1);
  WriteRegister(0x38); WriteData(x2);

  WriteRegister(0x20); WriteData(y1);
  WriteRegister(0x21); WriteData(x1);
  // SetCursorPos(x1, y1);
}

void DrawPixel(int16_t x, int16_t y, uint16_t color)
{
  setWindow(x, y, x, y);
  WriteRegister(0x22); WriteData(color);
}

void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  //uint16_t x0=x;
  uint16_t colortemp=POINT_COLOR;
  if(x>tft_W-16||y>tft_H-16)return;
  num=num-' ';
  setWindow(x,y,x+8-1,y+16-1);
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
        if(temp&0x01)DrawPixel(x+t,y+pos,POINT_COLOR);
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
  setWindow(x,y,x+14-1,y+24-1);
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
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
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
  setWindow(x,y,x+18-1,y+32-1);
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
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
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
  setWindow(x,y,x+26-1,y+48-1);
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
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
            DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
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
