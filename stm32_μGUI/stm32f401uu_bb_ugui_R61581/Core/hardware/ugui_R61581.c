/**
 *
 * ugui_R61581.c
 *
**/

#include "ugui_R61581.h"
#include "io_R61581.h"

#include "main.h"
#include "gpio.h"
#include "ugui.h"

uint16_t DISPLAY_WIDTH     =      320;
uint16_t DISPLAY_HEIGHT    =      480;

/* *** Glogal GUI structure for this. *** */
UG_GUI gui_R61581;

#pragma GCC push_options
#pragma GCC optimize("O3")
void
tft_write_data16(uint16_t data)
{
  tft_write_data(data >> 8);
  tft_write_data(data&0x00ff);
}

void
tft_write_data24(uint32_t data)
{
  tft_write_data(data >> 16);
  tft_write_data(data >> 8);
  tft_write_data(data & 0x00ff);
}

void
tft_write_reg16(uint16_t data)
{
  tft_write_reg(data >> 8);
  tft_write_reg(data&0x00ff);
}
#pragma GCC pop_options

void Display_Reset(void)
{
  tft_write_reg(0x01); // Software reset
  LL_mDelay(10);
}

void tft_SetRotation(uint8_t r)
{
  tft_write_reg(0x36);
  if(r == 0)
  {
    tft_write_data(0x40|0x08);
    DISPLAY_WIDTH = 320;
    DISPLAY_HEIGHT = 480;
  }
  if(r == 1)
  {
    tft_write_data(0x20|0x08);
    DISPLAY_WIDTH = 480;
    DISPLAY_HEIGHT = 320;
  }
  if(r == 2)
  {
    tft_write_data(0x80|0x08);
    DISPLAY_WIDTH = 320;
    DISPLAY_HEIGHT = 480;
  }
  if(r == 3)
  {
    tft_write_data(0x40|0x80|0x20|0x08);
    DISPLAY_WIDTH = 480;
    DISPLAY_HEIGHT = 320;
  }
}

void R61581_8bit(void)
{
  tft_write_reg(0x01); // Soft Reset
  LL_mDelay(150); // will power up with ONLY reset, sleep out, display on
  tft_write_reg(0x28); // Display Off
  tft_write_reg(0x3A); tft_write_data(0x55); // Pixel read=565, write=565.
  // // // // // // // // /
  tft_write_reg(0xB0); tft_write_data(0x00); // unlocks E0); F0
  tft_write_reg(0xB3); tft_write_data(0x02); tft_write_data(0x00); tft_write_data(0x00); tft_write_data(0x00); // Frame Memory, interface [02 00 00 00]
  tft_write_reg(0xB4); tft_write_data(0x00); // Frame mode [00]
  tft_write_reg(0xD0); tft_write_data(0x07); tft_write_data(0x42); tft_write_data(0x18); // Set Power [00 43 18] x1.00, x6, x3
  tft_write_reg(0xD1); tft_write_data(0x00); tft_write_data(0x07); tft_write_data(0x10); // Set VCOM  [00 00 00] x0.72, x1.02
  tft_write_reg(0xD2); tft_write_data(0x01); tft_write_data(0x02); // Set Power for Normal Mode [01 22]
  tft_write_reg(0xD3); tft_write_data(0x01); tft_write_data(0x02); // Set Power for Partial Mode [01 22]
  tft_write_reg(0xD4); tft_write_data(0x01); tft_write_data(0x02); // Set Power for Idle Mode [01 22]
  tft_write_reg(0xC0); tft_write_data(0x12); tft_write_data(0x3B); tft_write_data(0x00); tft_write_data(0x02); tft_write_data(0x11); // Panel Driving BGR for 1581 [10 3B 00 02 11]
  tft_write_reg(0xC1); tft_write_data(0x10); tft_write_data(0x10); tft_write_data(0x88); // Display Timing Normal [10 10 88]
  tft_write_reg(0xC5); tft_write_data(0x03); // Frame Rate [03]
  tft_write_reg(0xC6); tft_write_data(0x02); // Interface Control [02]
  tft_write_reg(0xC8); tft_write_data(0x00); tft_write_data(0x32); tft_write_data(0x36); tft_write_data(0x45); tft_write_data(0x06); tft_write_data(0x16); tft_write_data(0x37); tft_write_data(0x75); tft_write_data(0x77); tft_write_data(0x54); tft_write_data(0x0C); tft_write_data(0x00);
  tft_write_reg(0xCC); tft_write_data(0x00); // Panel Control [00]
  // // // // // // // // //
  tft_write_reg(0x11); // Sleep Out
  LL_mDelay(150);
  tft_write_reg(0x29); // Display On
}

void Display_Init(void)
{
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  LL_mDelay(5);
  LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  LL_mDelay(15);
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  LL_mDelay(15);
  LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  LL_GPIO_SetOutputPin(LCD_RD_GPIO_Port, LCD_RD_Pin);
  R61581_8bit();
  // Initialize global structure and set PSET to this.PSET.
  UG_Init(&gui_R61581, Display_PSet, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  // Register accelerators.
  UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
  UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
  UG_DriverRegister(DRIVER_DRAW_IMAGE, (void*)HW_DrawImage);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
void Display_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y)
{
  tft_write_reg(0x2a); // SET column address
  // printf ("data: %X, %X, %X, %X\n",data[0],data[1],data[2],data[3]);
  tft_write_data16(s_x);
  tft_write_data16(e_x);
  tft_write_reg(0x2b); // SET row address
  tft_write_data16(s_y);
  tft_write_data16(e_y);
  tft_write_reg(0x2c);
}
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{
  if((x < 0) ||(x >= DISPLAY_WIDTH) || (y < 0) || (y >= DISPLAY_HEIGHT)) return;

  Display_WindowSet(x, x + 1, y, y + 1);

  #ifdef USE_COLOR_RGB888 // 24-bit colour 8*Red-8*Green-8*Blue
    tft_write_data24(c);
  #endif

  #ifdef USE_COLOR_RGB565 // 16-bit colour 5*Red-6*Green-5*Blue
    tft_write_data16(c);
  #endif
}
#pragma GCC pop_options

UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
  uint16_t loopx, loopy;
  if((x1 < 0) ||(x1 >= DISPLAY_WIDTH) || (y1 < 0) || (y1 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= DISPLAY_WIDTH) || (y2 < 0) || (y2 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;

  Display_WindowSet(x1,x2,y1,y2);

  for (loopx = x1; loopx < x2 + 1; loopx++)
  {
    for (loopy = y1; loopy < y2 + 1; loopy++)
    {
      #ifdef USE_COLOR_RGB888 // 24-bit colour 8*Red-8*Green-8*Blue
        tft_write_data24(c);
      #endif

      #ifdef USE_COLOR_RGB565 // 16-bit colour 5*Red-6*Green-5*Blue
        tft_write_data16(c);
      #endif
    }
  }
  return UG_RESULT_OK;
}

UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c )
{
  if((x1 < 0) ||(x1 >= DISPLAY_WIDTH) || (y1 < 0) || (y1 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= DISPLAY_WIDTH) || (y2 < 0) || (y2 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;
  // If it is a vertical or a horizontal line, draw it.
  // If not, then use original drawline routine.

  if ((x1 == x2) || (y1 == y2))
  {
    HW_FillFrame(x1, y1, x2, y2, c);
    return UG_RESULT_OK;
  }
  return UG_RESULT_FAIL;
}

UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize)
{
  if((x1 < 0) ||(x1 >= DISPLAY_WIDTH) || (y1 < 0) || (y1 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;
  if((x2 < 0) ||(x2 >= DISPLAY_WIDTH) || (y2 < 0) || (y2 >= DISPLAY_HEIGHT)) return UG_RESULT_FAIL;
  Display_WindowSet(x1,x2,y1,y2);

  uint16_t *ptr = image;

  #ifdef USE_COLOR_RGB888 // 24-bit colour 8*Red-8*Green-8*Blue
  for(uint16_t i=0 ;i < pSize; i++)
  {
    tft_write_data24(*(ptr++));
  }
  #endif

  #ifdef USE_COLOR_RGB565 // 16-bit colour 5*Red-6*Green-5*Blue

  for(uint16_t i=0 ;i < pSize; i++)
  {
    tft_write_data16(*(ptr++));
  }
  #endif

  return UG_RESULT_OK;
}

/* [] END OF FILE */
