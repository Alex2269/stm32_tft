/*******************************************************************************
* File Name: ugui_SSD1963.c
* driver SSD1963 paralel 8 bit interface
*******************************************************************************/

#include "stdio.h"
#include "stdbool.h"
#include "io_SSD1963.h"
#include "main.h"
#include "gpio.h"
#include "ugui.h"
#include "ugui_SSD1963.h"

uint16_t DISPLAY_WIDTH = 800;
uint16_t DISPLAY_HEIGHT = 480;
UG_GUI gui1963;
uint16_t rotation;

uint16_t Display_Get_WIDTH(void)
{
  return DISPLAY_WIDTH;
}

uint16_t Display_Get_HEIGHT(void)
{
  return DISPLAY_HEIGHT;
}

void Display_Set_WIDTH(uint16_t x)
{
  DISPLAY_WIDTH = x;
}

void Display_Set_HEIGHT(uint16_t y)
{
  DISPLAY_HEIGHT = y;
}

void Display_Set_rotation(uint8_t r)
{
  rotation = r;
  // --
  Display_WriteCommand(0xb0); // SET LCD MODE
  Display_WriteData(0x8e); // 24-bit panel
  Display_WriteData(0x00); // SET TFT MODE / SET Hsync + Vsync + DE MODE
  // --
  if(rotation == 1 || rotation == 2)
  {
    Display_Set_WIDTH(800);
    Display_Set_HEIGHT(480);
    // --
    Display_WriteData16(Display_Get_WIDTH()-1); // SET 480 Horizontal size
    Display_WriteData16(Display_Get_HEIGHT()-1); // SET 800 Vertical size
    Display_WriteData(0x00); // SET Even/Odd line RGB sequence = RGB
    // --
    Display_WriteCommand(0x36); // Set address mode
    Display_WriteData(r); // 1 Normal 2 Flipped
  }
  if(rotation == 0 || rotation == 3)
  {
    Display_Set_WIDTH(480);
    Display_Set_HEIGHT(800);
    // --
    Display_WriteData16(Display_Get_HEIGHT()-1); // SET 800 Horizontal size
    Display_WriteData16(Display_Get_WIDTH()-1); // SET 480 Vertical size
    Display_WriteData(0x00); // SET Even/Odd line RGB sequence = RGB
    // --
    Display_WriteCommand(0x36); // Set address mode
    Display_WriteData(r | 0b00100000); // rotate 90 degree
  }
}

void Display_Init(uint16_t r)
{
  rotation = r;
  // --
  Display_WriteCommand(0x01); // Software reset
  Display_WriteData(0x23); // PLL = 120MHz
  Display_WriteData(0x02);
  Display_WriteData(0x04);
  // --
  Display_WriteCommand(0xe0);
  Display_WriteData(0x01); // Enable PLL
  LL_mDelay(10);
  // --
  Display_WriteCommand(0xe0);
  Display_WriteData(0x03); // Lock PLL
  LL_mDelay(10);
  // --
  Display_WriteCommand(0xe6); // SET PCLK Freq = 33.26MHz
  Display_WriteData(0x07);
  Display_WriteData(0xff);
  Display_WriteData(0xff);
  // --
  Display_WriteCommand(0xb4); // SET HBP
  Display_WriteData(0x03); // SET HT Hsync Total = 928 HightByte
  Display_WriteData(0xa0); // SET HT Hsync Total = 928 LowByte
  Display_WriteData(0x00); // SET HPS 0 HightByte
  Display_WriteData(0x2e); // SET HPS 46 LowByte
  Display_WriteData(0x30); // SET HPW 48
  Display_WriteData(0x00); // SET LPS Hsync pulse start position 0 HighByte
  Display_WriteData(0x0f); // SET LPS Hsync pulse start position 15 LowByte
  Display_WriteData(0x00); // SET LPSPP Hsync pulse subpixel start position
  // --
  Display_WriteCommand(0xb6); // SET VBP
  Display_WriteData(0x02); // SET VT Vsync total = 525 HighByte
  Display_WriteData(0x0d); // SET VT Vsync total = 525 LowByte
  Display_WriteData(0x00); // SET VPS 0 HighByte
  Display_WriteData(0x10); // SET VPS 16 LowByte
  Display_WriteData(0x10); // SET VPW Vsync pulse 16 Byte
  Display_WriteData(0x00); // SET FPS Vsync pulse start position 0 HighByte
  Display_WriteData(0x19); // SET FPS Vsync pulse start position 25 LowByte
  // --
  Display_WriteCommand(0xf0);
  Display_WriteData(0x00); // SET pixel data I/F format=8bit
  // --
  Display_WriteCommand(0x29); // SET display on
  LL_mDelay(10);
  // --
  // Initialize global structure and set PSET to this.PSET.
  UG_Init(&gui1963, Display_PSet, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  // --
  // Register acceleratos.
  UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
  UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
  UG_DriverRegister(DRIVER_DRAW_IMAGE, (void*)HW_DrawImage);
  Display_Set_rotation(rotation);
  Display_Set_backlight(75);
}

void Display_Set_backlight(uint8_t percent)
{
  // The SSD1963 has a built-in PWM (duty_cycle 00..FF).
  // Its output can be used by a Dynamic Background Control or by a host (user)
  // Check your LCD's hardware, the PWM connection is default left open and instead
  // connected to a LED connection on the breakout board
  // --
  Display_WriteCommand(0xBE); // Set PWM for BackLight
  Display_WriteData(0x04);
  // Display_WriteData(0x0E); // PWMF[7:0] = 2, PWM base freq = PLL/(256*(1+5))/256 = 300Hz for a PLL freq = 120MHz (source: Displaytech)
  if (percent == 0xFF) // use percent==0xFF to turn off SSD1963 pwm in power SLEEP or DEEP SLEEP mode
    Display_WriteData(0x00);
  else if (percent >= 100)
    Display_WriteData(0xFF); // Fully on for any percentage >= 100
  else
    Display_WriteData((percent*255)/100 & 0x00FF);
  // --
  Display_WriteData(0x01); // Controlled by host (not DBC), enabled
  Display_WriteData(0xFF); // DBC manual brightness (not used - zero would work)
  Display_WriteData(0x00); // DBC minimum brightness
  Display_WriteData(0x01); // Brightness prescaler - active when Transition Effect enable A5 = 1
}

void Display_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y)
{
  if(rotation == 0 || rotation == 3)
  {
    Display_WriteCommand(0x2a); // SET column address
    Display_WriteData16(s_y); // SET start column address
    Display_WriteData16(e_y); // SET end column address
    // --
    Display_WriteCommand(0x2b); // SET row address
    Display_WriteData16(s_x); // SET start row address
    Display_WriteData16(e_x); // SET end row address
    // --
    Display_WriteCommand(0x2c); // WRITE memory start
  }
  if(rotation == 1 || rotation == 2)
  {
    Display_WriteCommand(0x2a); // SET column address
    Display_WriteData16(s_x); // SET start column address
    Display_WriteData16(e_x); // SET end column address
    // --
    Display_WriteCommand(0x2b); // SET row address
    Display_WriteData16(s_y); // SET start row address
    Display_WriteData16(e_y); // SET end row address
    // --
    Display_WriteCommand(0x2c); // WRITE memory start
  }
}

void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{
  if((x < 0) ||(x >= Display_Get_WIDTH()) || (y < 0) || (y >= Display_Get_HEIGHT())) return;
  Display_WindowSet(x, x + 1, y, y + 1);
  Display_WriteData24(c);
}

UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
  uint16_t loopx, loopy;
  // --
  if((x1 < 0) ||(x1 >= Display_Get_WIDTH()) || (y1 < 0) || (y1 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= Display_Get_WIDTH()) || (y2 < 0) || (y2 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  // --
  Display_WindowSet(x1,x2,y1,y2);
  // --
  Display_WriteCommand(0x2c);
  for (loopx = x1; loopx < x2 + 1; loopx++)
  {
    for (loopy = y1; loopy < y2 + 1; loopy++)
    {
      Display_WriteData24(c);
    }
  }
  // --
  return UG_RESULT_OK;
}

UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c )
{
  if((x1 < 0) ||(x1 >= Display_Get_WIDTH()) || (y1 < 0) || (y1 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= Display_Get_WIDTH()) || (y2 < 0) || (y2 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  // --
  // If it is a vertical or a horizontal line, draw it.
  // If not, then use original drawline routine.
  if ((x1 == x2) || (y1 == y2))
  {
    HW_FillFrame(x1, y1, x2, y2, c);
    return UG_RESULT_OK;
  }
  // --
  return UG_RESULT_FAIL;
}

UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint8_t *image, uint16_t pSize)
{
  if((x1 < 0) ||(x1 >= Display_Get_WIDTH()) || (y1 < 0) || (y1 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= Display_Get_WIDTH()) || (y2 < 0) || (y2 >= Display_Get_HEIGHT())) return UG_RESULT_FAIL;
  Display_WindowSet(x1,x2,y1,y2);
  // --
  uint16_t *ptr = image;
  Display_WriteCommand(0x2c);
  #ifdef USE_COLOR_RGB888 // 24-bit colour 8*Red-8*Green-8*Blue
  for(uint16_t i=0 ;i < pSize; i++)
  {
    Display_WriteData24(*(ptr++));
  }
  #endif
  // --
  #ifdef USE_COLOR_RGB565 // 16-bit colour 5*Red-6*Green-5*Blue
  for(uint16_t i=0 ;i < pSize; i++)
  {
    Display_WriteData24(*(ptr++));
  }
  #endif
  // --
  return UG_RESULT_OK;
}

/* [] END OF FILE */
