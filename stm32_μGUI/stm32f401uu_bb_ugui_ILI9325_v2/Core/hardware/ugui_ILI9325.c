/**
 *
 * ugui_ILI9325.c
 *
**/

// #include "stm32f401xc.h"
#include <io_ILI9325.h>
#include <ugui_ILI9325.h>

#include "main.h"
#include "gpio.h"
#include "ugui.h"

uint16_t _width;
uint16_t _height;
uint8_t _rotation;

/* *** Glogal GUI structure for this. *** */
UG_GUI gui_ILI9325;

static inline void
delay_us(uint32_t volatile microsec)
{
  microsec *=(SystemCoreClock/1000000)/5;
  while(microsec--);
}

static inline void
delay(uint32_t volatile msec)
{
  msec *=(SystemCoreClock/1000)/5;
  while(msec--);
}

uint16_t get_width(void)
{
  return _width;
}

uint16_t get_height(void)
{
  return _height;
}

void set_width(uint16_t width)
{
  _width = width;
}

void set_height(uint16_t height)
{
  _height = height;
}

void Display_Reset()
{
  tft_write_reg(0x01); // Software reset
  delay(10);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
static inline void
tft_write_data16(uint16_t data)
{
  tft_write_data(data >> 8);
  tft_write_data(data&0x00ff);
}

static inline void
tft_write_data24(uint32_t data)
{
  tft_write_data(data >> 16);
  tft_write_data(data >> 8);
  tft_write_data(data & 0x00ff);
}

static inline void
tft_write_reg16(uint16_t data)
{
  tft_write_reg(data >> 8);
  tft_write_reg(data&0x00ff);
}

static inline void
tft_write_reg16_data16(uint16_t addr, uint16_t data)
{
  tft_write_reg(addr >> 8);
  tft_write_reg(addr & 0x00ff);
  tft_write_data(data >> 8);
  tft_write_data(data & 0x00ff);
}
#pragma GCC pop_options

void ili9325_init(void)
{
  tft_write_reg(0x01); // soft Reset
  delay(100);
  tft_write_reg(0x28); // display Off
  tft_write_reg(0x3A); tft_write_data(0x55); // pixel read=565, write=565.
  delay(50);

  tft_write_reg16(0x00E3); tft_write_data16(0x3008); // Set internal timing
  tft_write_reg16(0x00E7); tft_write_data16(0x0012); // Set internal timing
  tft_write_reg16(0x00EF); tft_write_data16(0x1231); // Set internal timing
  tft_write_reg16(0x0001); tft_write_data16(0x0100); // set SS and SM bit
  tft_write_reg16(0x0002); tft_write_data16(0x0700); // set 1 line inversion
  tft_write_reg16(0x0003); tft_write_data16(0x1030); // /* set GRAM write direction and BGR=1 */
  // 0x0003); tft_write_data16(0x0030); // set GRAM write direction and RGB=1.
  tft_write_reg16(0x0004); tft_write_data16(0x0000); // Resize register
  tft_write_reg16(0x0008); tft_write_data16(0x0207); // set the back porch and front porch
  tft_write_reg16(0x0009); tft_write_data16(0x0000); // set non-display area refresh cycle ISC[3:0]
  tft_write_reg16(0x000A); tft_write_data16(0x0000); // FMARK function
  tft_write_reg16(0x000C); tft_write_data16(0x0000); // RGB interface setting
  tft_write_reg16(0x000D); tft_write_data16(0x0000); // Frame marker Position
  tft_write_reg16(0x000F); tft_write_data16(0x0000); // RGB interface polarity
  //*************Power On sequence ****************//
  tft_write_reg16(0x0010); tft_write_data16(0x0000); // SAP); // BT[3:0]); // AP); // DSTB); // SLP); // STB
  tft_write_reg16(0x0011); tft_write_data16(0x0007); // DC1[2:0]); // DC0[2:0]); // VC[2:0]
  tft_write_reg16(0x0012); tft_write_data16(0x0000); // VREG1OUT voltage
  tft_write_reg16(0x0013); tft_write_data16(0x0000); // VDV[4:0] for VCOM amplitude
  delay(200); // Dis-charge capacitor power voltage
  tft_write_reg16(0x0010); tft_write_data16(0x1290); // SAP); // BT[3:0]); // AP); // DSTB); // SLP); // STB
  tft_write_reg16(0x0011); tft_write_data16(0x0227); // DC1[2:0]); // DC0[2:0]); // VC[2:0]
  delay(50);
  tft_write_reg16(0x0012); tft_write_data16(0x0019); // Internal reference voltage= Vci;
  delay(50);
  tft_write_reg16(0x0013); tft_write_data16(0x1D00); // Set VDV[4:0] for VCOM amplitude
  tft_write_reg16(0x0029); tft_write_data16(0x0028); // Set VCM[5:0] for VCOMH
  tft_write_reg16(0x002B); tft_write_data16(0x000C); // Set Frame Rate
  delay(50);
  tft_write_reg16(0x0020); tft_write_data16(0x0000); // GRAM horizontal Address
  tft_write_reg16(0x0021); tft_write_data16(0x0000); // GRAM Vertical Address
  // ----------- Adjust the Gamma Curve ----------//
  tft_write_reg16(0x0030); tft_write_data16(0x0000); //
  tft_write_reg16(0x0031); tft_write_data16(0x0603); //
  tft_write_reg16(0x0032); tft_write_data16(0x0206); //
  tft_write_reg16(0x0035); tft_write_data16(0x0206); //
  tft_write_reg16(0x0036); tft_write_data16(0x0004); //
  tft_write_reg16(0x0037); tft_write_data16(0x0105); //
  tft_write_reg16(0x0038); tft_write_data16(0x0401); //
  tft_write_reg16(0x0039); tft_write_data16(0x0707); //
  tft_write_reg16(0x003C); tft_write_data16(0x0602); //
  tft_write_reg16(0x003D); tft_write_data16(0x0004); //
  //------------------ Set GRAM area ---------------//
  tft_write_reg16(0x0050); tft_write_data16(0x0000); // Horizontal GRAM Start Address
  tft_write_reg16(0x0051); tft_write_data16(0x00EF); // Horizontal GRAM End Address
  tft_write_reg16(0x0052); tft_write_data16(0x0000); // Vertical GRAM Start Address
  tft_write_reg16(0x0053); tft_write_data16(0x013F); // Vertical GRAM Start Address
  tft_write_reg16(0x0060); tft_write_data16(0xA700); // Gate Scan Line
  tft_write_reg16(0x0061); tft_write_data16(0x0001); // NDL); //VLE); // REV
  tft_write_reg16(0x006A); tft_write_data16(0x0000); // set scrolling line
  //-------------- Partial Display Control ---------//
  tft_write_reg16(0x0080); tft_write_data16(0x0000); //
  tft_write_reg16(0x0081); tft_write_data16(0x0000); //
  tft_write_reg16(0x0082); tft_write_data16(0x0000); //
  tft_write_reg16(0x0083); tft_write_data16(0x0000); //
  tft_write_reg16(0x0084); tft_write_data16(0x0000); //
  tft_write_reg16(0x0085); tft_write_data16(0x0000); //
  //-------------- Panel Control -------------------//
  tft_write_reg16(0x0090); tft_write_data16(0x0010); //
  tft_write_reg16(0x0092); tft_write_data16(0x0600); //
  tft_write_reg16(0x0007); tft_write_data16(0x0133); // 262K color and display ON

  tft_write_reg(0x11); // sleep Out
  delay(50);
  tft_write_reg(0x29); // display On
}

void Display_Init()
{
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(5);
  LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(15);
  LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
  delay(15);
  LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
  LL_GPIO_SetOutputPin(LCD_RD_GPIO_Port, LCD_RD_Pin);
  ili9325_init();
  tft_SetRotation(0);
  invertDisplay(0);
  // Initialize global structure and set PSET to this.PSET.
  UG_Init(&gui_ILI9325, Display_PSet, get_width(), get_height());
  // Register accelerators.
  UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
  UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
  UG_DriverRegister(DRIVER_DRAW_IMAGE, (void*)HW_DrawImage);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
void Display_WindowSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  if(x1 < 0 || x2 >= get_width()) return;
  if(y1 < 0 || y2 >= get_height()) return;

  switch (_rotation)
  {
    case 0: // PORTRAIT:
    case 2: // PORTRAIT_REV: 180 degrees
      tft_write_reg16_data16(0x0020, x1);
      tft_write_reg16_data16(0x0021, y1);
      if(!(1 == x2 && y1 == y2))
      {
        tft_write_reg16_data16(0x0050, x1);
        tft_write_reg16_data16(0x0052, y1);
        tft_write_reg16_data16(0x0051, x2);
        tft_write_reg16_data16(0x0053, y2);
      }
      break;
    case 1: // LANDSCAPE: 90 degrees
    case 3: // LANDSCAPE_REV: 270 degrees
      tft_write_reg16_data16(0x0021, x1);
      tft_write_reg16_data16(0x0020, y1);
      if(!(x1 == x2 && y1 == y2))
      {
        tft_write_reg16_data16(0x0052, x1);
        tft_write_reg16_data16(0x0050, y1);
        tft_write_reg16_data16(0x0053, x2);
        tft_write_reg16_data16(0x0051, y2);
      }
      break;
  }
}
#pragma GCC pop_options

void vertScroll(uint16_t top, uint16_t scrollines, uint16_t offset)
{
  int16_t vsp;
  if(offset <= -scrollines || offset >= scrollines) offset = 0; // valid scroll
  vsp = top + offset; // vertical start position
  if(offset < 0)  vsp += scrollines; // eep in unsigned range

  tft_write_reg16_data16(0x61, 3); // !NDL, VLE, REV
  tft_write_reg16_data16(0x6A, vsp); // VL#
}

void invertDisplay(bool i)
{
  if(i) tft_write_reg16(0x21);
  else tft_write_reg16(0x20);
  tft_write_reg16_data16(0x61, 1);
}

void tft_SetRotation(uint8_t r)
{
  // uint8_t val;
  _rotation = r & 3; // just perform the operation ourselves on the protected variables

  switch (_rotation)
  {
    case 0: // PORTRAIT:
      // val = (0x08); // Y=0, MX=1, MV=0, ML=0, BGR=1
      set_height(320);
      set_width(240);
      tft_write_reg16_data16(0x0060, 0xA700);
      tft_write_reg16_data16(0x0001, 0x0100);
      tft_write_reg16_data16(0x0003, 0x1030);
      break;
    case 1: // LANDSCAPE: 90 degrees
      // val = (0x20|0x08); // Y=0, MX=0, MV=1, ML=0, BGR=1
      set_height(240);
      set_width(320);
      tft_write_reg16_data16(0x0060, 0xA700);
      tft_write_reg16_data16(0x0001, 0);
      tft_write_reg16_data16(0x0003, 0x1038);
      break;
    case 2: // PORTRAIT_REV: 180 degrees
      // val = (0x08); // Y=0, MX=1, MV=0, ML=0, BGR=1
      set_height(320);
      set_width(240);
      tft_write_reg16_data16(0x0060, 0x2700);
      tft_write_reg16_data16(0x0001, 0x0000);
      tft_write_reg16_data16(0x0003, 0x1030);
      break;
    case 3: // LANDSCAPE_REV: 270 degrees
      // val = (0x20|0x08); // Y=0, MX=0, MV=1, ML=0, BGR=1
      set_height(240);
      set_width(320);
      tft_write_reg16_data16(0x0060, 0x2700);
      tft_write_reg16_data16(0x0001, 0x0100);
      tft_write_reg16_data16(0x0003, 0x1038);
      break;
  }
  Display_WindowSet(0, get_width() - 1, 0, get_height() - 1);
  vertScroll(0, get_height(), 0); // reset scrolling after a rotation
}

#pragma GCC push_options
#pragma GCC optimize("O3")
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{
  if((x < 0) ||(x >= get_width()) || (y < 0) || (y >= get_height())) return;
  
  Display_WindowSet(x, y, x+1, y+1);
  tft_write_reg(0x0022);

  tft_write_data(c >> 8);
  tft_write_data(c & 0x00ff);
}
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
  uint16_t loopx, loopy;
  if((x1 < 0) || (x1 >= get_width()) || (y1 < 0) || (y1 >= get_height())) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= get_width()) || (y2 < 0) || (y2 >= get_height())) return UG_RESULT_FAIL;

  Display_WindowSet(x1,y1,x2,y2);
  tft_write_reg(0x0022);
  tft_write_data16(c);

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
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize("O3")
UG_RESULT HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
  if((x1 < 0) || (x1 >= get_width()) || (y1 < 0) || (y1 >= get_height())) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= get_width()) || (y2 < 0) || (y2 >= get_height())) return UG_RESULT_FAIL;

  // If it is a vertical or a horizontal line, draw it.
  // If not, then use original drawline routine.

  if ((x1 == x2) || (y1 == y2))
  {
    HW_FillFrame(x1, y1, x2, y2, c);
    return UG_RESULT_OK;
  }
  return UG_RESULT_FAIL;
}
#pragma GCC pop_options


UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize)
{
  if((x1 < 0) || (x1 >= get_width()) || (y1 < 0) || (y1 >= get_height())) return UG_RESULT_FAIL;
  if((x2 < 0) || (x2 >= get_width()) || (y2 < 0) || (y2 >= get_height())) return UG_RESULT_FAIL;
  Display_WindowSet(x1,x2,y1,y2);

  Display_WindowSet(x1,y1,x2,y2);
  tft_write_reg(0x0022);

  uint16_t *ptr = image;
  #ifdef USE_COLOR_RGB888 // 24-bit colour 8*Red-8*Green-8*Blue
  for(uint16_t i=0 ;i < pSize; i++)
  {
    tft_write_data24((uint32_t)*(ptr++));
  }
  #endif

  #ifdef USE_COLOR_RGB565 // 16-bit colour 5*Red-6*Green-5*Blue
  for(uint16_t i=0 ;i < pSize; i++)
  {
    tft_write_data16((uint16_t)*(ptr++));
  }
  #endif

  return UG_RESULT_OK;
}


/* [] END OF FILE */
