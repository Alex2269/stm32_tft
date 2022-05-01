#include "stm32f1xx_hal.h"
#include "st7775.h"

uint8_t ScrollIndex = 0;

void WriteRegister(uint8_t IndexRegister)
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, 0);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);
  GPIOA->ODR = IndexRegister;
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);
}

void WriteData(uint8_t HiData, uint8_t LoData)
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, 1);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);
  GPIOA->ODR = HiData;
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);
  GPIOA->ODR = LoData;
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 0);
  HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, 1);  
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);
}

void DrawPixel(uint8_t X, uint8_t Y, uint16_t Color)
{
  WriteRegister(0x37);
  WriteData(0, 0);
  WriteRegister(0x36);
  WriteData(0, 0xAF);
  WriteRegister(0x39);
  WriteData(0, 0);
  WriteRegister(0x38);
  WriteData(0, 0xDB);
  
  SetCursorPos(X, Y);
  WriteRegister(0x22); //DRAM write data
  WriteColor(Color);
}

void DrawHLine(uint8_t X, uint8_t Y, uint8_t Width, uint16_t Color)
{
  WriteRegister(0x37);
  WriteData(0, Y);
  WriteRegister(0x36);
  WriteData(0, Y);
  WriteRegister(0x39);
  WriteData(0, X);
  WriteRegister(0x38);
  WriteData(0, X + Width);
  
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
  WriteData(0, Y);
  WriteRegister(0x36);
  WriteData(0, Y + ((8 * Scale) - 1));
  WriteRegister(0x39);
  WriteData(0, X);
  WriteRegister(0x38);
  WriteData(0, X + ((8 * Scale) - 1));
  
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
  WriteRegister(0x20);
  WriteData(0, 0);
  WriteRegister(0x21);
  WriteData(0, 0);
  WriteRegister(0x22);
  for(uint8_t i = 0 ; i < 220u ; ++i)
  {
    for(uint8_t j = 0 ; j < 176u ; ++j)
    {
      WriteData(0, 0);
    }
  }
}

void DrawArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint16_t Color)
{
  WriteRegister(0x37);
  WriteData(0, Y);
  WriteRegister(0x36);
  WriteData(0, Y + Height - 1);
  WriteRegister(0x39);
  WriteData(0, X);
  WriteRegister(0x38);
  WriteData(0, X + Width - 1);
  
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
  WriteData(0, ScrollIndex);
}

void ScrollRight()
{
  if(--ScrollIndex == 255u)
    ScrollIndex = 219u;
  WriteRegister(0x33);
  WriteData(0, ScrollIndex);
}

void InitDisplay()
{
  HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 0);  //Reset screen
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0);  //Reset screen
  HAL_Delay(25);
  HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 1);  //Reset screen
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1);  //Reset screen
  HAL_Delay(10);
  //Backlight on
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
  HAL_Delay(1);

  WriteRegister(0x01);
  WriteData(0, 0b00011100); //Sets number of lines used (220 lines)
  WriteRegister(0x02);
  WriteData(0, 0); //Column or dot inversion (column)
  WriteRegister(0x03);
  WriteData(0b00010000, 0b00111000); //RGB, screen orientation
  WriteRegister(0x08);
  WriteData(0b00001000, 0b00001000); //Front and back porch period (8 lines each)
  WriteRegister(0x0B);
  WriteData(0, 0); //Internal clock synchronization (0 clocks)
  WriteRegister(0x0C);
  WriteData(0, 0b00000000); //RGB mode (16 bit)
  WriteRegister(0x0F);
  WriteData(0, 0b00000001); //Oscillator (on)
  HAL_Delay(10); //Wait at least 10ms for ocillator to stabilize
  
  WriteRegister(0x10);
  WriteData(0, 0); //Power (no sleep, no standby)
  WriteRegister(0x11);
  WriteData(0b00010000, 0); //Booster sequence circuit (on)

  WriteRegister(0xB0);
  WriteData(0x14, 0x11); //Power control
  WriteRegister(0xB1);
  WriteData(0x02, 0x02);
  WriteRegister(0xB2);
  WriteData(0x03, 0x13);
  
  WriteRegister(0x30);
  WriteData(0, 0); //Gate line start
  WriteRegister(0x31);
  WriteData(0, 0xDB); //Vertical scroll end (line 219)
  WriteRegister(0x32);
  WriteData(0, 0); //Verticall scroll start (line 0)
  WriteRegister(0x33);
  WriteData(0, 0); //Number of scrolling lines (disable scrolling)
  
  WriteRegister(0x34);
  WriteData(0, 0xDB); //Partial screen position end (219 lines)
  WriteRegister(0x35);
  WriteData(0, 0); //Partial screen position start
  
  WriteRegister(0x36);
  WriteData(0, 0xAF); //Horizontal end (175)
  WriteRegister(0x37);
  WriteData(0, 0); //Horizontal start (0)
  WriteRegister(0x38);
  WriteData(0, 0xDB); //Vertical start (219)
  WriteRegister(0x39);
  WriteData(0, 0); //Vertical end (0)
  
  //Gamma control
  /*WriteRegister(0xFF);
  WriteData(0, 0x03);
  WriteRegister(0x50);
  WriteData(0, 0);
  WriteRegister(0x51);
  WriteData(0x03, 0);
  WriteRegister(0x52);
  WriteData(0x01, 0x03);
  WriteRegister(0x53);
  WriteData(0x20, 0x11);
  WriteRegister(0x54);
  WriteData(0x07, 0x03);
  WriteRegister(0x55);
  WriteData(0, 0);
  WriteRegister(0x56);
  WriteData(0x04, 0);
  WriteRegister(0x57);
  WriteData(0x01, 0x07);
  WriteRegister(0x58);
  WriteData(0x20, 0x11);
  WriteRegister(0x59);
  WriteData(0x07, 0x03);*/
  
  WriteRegister(0x20);
  WriteData(0, 0);
  WriteRegister(0x21);
  WriteData(0, 0);

  WriteRegister(0x07);
  WriteData(0, 0b00010111); //Frame tearing (allowed), Grayscale (off), Display (on)

  HAL_Delay(10);

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
