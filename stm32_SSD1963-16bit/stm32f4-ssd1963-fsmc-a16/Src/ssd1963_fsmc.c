#include "ssd1963_fsmc.h"

void tft_Delay(uint32_t delay)
{
  uint32_t i;
  for(i=0;i<delay;i++);
}

inline uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

inline static uint16_t H24_RGB565(uint8_t reverse, uint32_t color24)
{
  uint8_t b = (color24 >> 16) & 0xFF;
  uint8_t g = (color24 >> 8) & 0xFF;
  uint8_t r = color24 & 0xFF;
  if (reverse) return ((b / 8) << 11) | ((g / 4) << 5) | (r / 8);
  else return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

inline static void LCD_Send_Cmd(uint16_t data)
{
  pin_low(RS_PORT,RS_PIN);
  ADDR_CMD = (data);
  tft_Delay(1);
}

inline static void LCD_Send_Dat(uint16_t data)
{
  pin_high(RS_PORT,RS_PIN);
  ADDR_DATA = (data);
  tft_Delay(1);
}

// inline static uint16_t LCD_Read_Data()
//{
// uint16_t data = ADDR_DATA;
// return data; 
//}

// inline static uint16_t LCD_Read_Reg(uint16_t reg_addr)
//{
// volatile uint16_t data = 0;
// LCD_Send_Cmd(reg_addr);
// data = LCD_Read_Data();
// return data;
//}

inline static void LCD_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  LCD_Send_Cmd(LCD_COLUMN_ADDR);
  LCD_Send_Dat(y1 >> 8);
  LCD_Send_Dat(y1 & 0x00FF);
  LCD_Send_Dat(y2 >> 8);
  LCD_Send_Dat(y2 & 0x00FF);
  LCD_Send_Cmd(LCD_PAGE_ADDR);
  LCD_Send_Dat(x1 >> 8);
  LCD_Send_Dat(x1 & 0x00FF);
  LCD_Send_Dat(x2 >> 8);
  LCD_Send_Dat(x2 & 0x00FF);
  LCD_Send_Cmd(LCD_GRAM);
}

void LCD_Pixel(uint16_t x, uint16_t y, uint32_t color24)
{
  LCD_Window(x, y, x, y);
  LCD_Send_Dat(H24_RGB565(1, color24));
}

void LCD_Rect_Fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color24)
{
  uint32_t i = 0;
  LCD_Window(y, x, y + h - 1, x + w - 1);
  for (i = 0; i < w * h; i++) LCD_Send_Dat(H24_RGB565(1, color24));
}

void LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint32_t color24)
{
  int deltaX = abs(x2 - x1);
  int deltaY = abs(y2 - y1);
  int signX = x1 < x2 ? 1 : -1;
  int signY = y1 < y2 ? 1 : -1;
  int error = deltaX - deltaY;
  int error2 = 0;
  for (;;)
  {
    LCD_Rect_Fill(x1, y1, size, size, color24);
    if (x1 == x2 && y1 == y2)
    break;
    error2 = error * 2;
    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }
}

void LCD_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint32_t color24)
{
  LCD_Line(x1, y1, x2, y2, size, color24);
  LCD_Line(x2, y2, x3, y3, size, color24);
  LCD_Line(x3, y3, x1, y1, size, color24);
}

void LCD_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint32_t color24)
{
  LCD_Line(x, y, x + w, y, size, color24);
  LCD_Line(x, y + h, x + w, y + h, size, color24);
  LCD_Line(x, y, x, y + h, size, color24);
  LCD_Line(x + w, y, x + w, y + h, size, color24);
}

void LCD_Ellipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint8_t fill, uint8_t size, uint32_t color24)
{
  int16_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;
  if (fill)
  {
    for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++)
    {
      LCD_Line(x0 - x, y0 - y, x0 + x + 1 - size, y0 - y, size, color24);
      LCD_Line(x0 - x, y0 + y, x0 + x + 1 - size, y0 + y, size, color24);
      if (s >= 0)
      {
        s += fx2 * (1 - y);
        y--;
      }
      s += ry2 * ((4 * x) + 6);
    }
    for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1-2 * rx); rx2 * y <= ry2 * x; y++)
    {
      LCD_Line(x0 - x, y0 - y, x0 + x + 1 - size, y0 - y, size, color24);
      LCD_Line(x0 - x, y0 + y, x0 + x + 1 - size, y0 + y, size, color24);
      if (s >= 0)
      {
        s += fy2 * (1 - x);
        x--;
      }
      s += rx2 * ((4 * y) + 6);
    }
  }
  else
  {
    for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++)
    {
      LCD_Rect_Fill(x0 + x, y0 + y, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 + y, size, size, color24);
      LCD_Rect_Fill(x0 + x, y0 - y, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 - y, size, size, color24);
      if (s >= 0)
      {
        s += fx2 * (1 - y);
        y--;
      }
      s += ry2 * ((4 * x) + 6);
    }
    for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++)
    {
      LCD_Rect_Fill(x0 + x, y0 + y, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 + y, size, size, color24);
      LCD_Rect_Fill(x0 + x, y0 - y, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 - y, size, size, color24);
      if (s >= 0)
      {
        s += fy2 * (1 - x);
        x--;
      }
      s += rx2 * ((4 * y) + 6);
    }
  }
}

void LCD_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint32_t color24)
{
  int a_, b_, P;
  a_ = 0;
  b_ = radius;
  P = 1 - radius;
  while (a_ <= b_)
  {
    if (fill == 1)
    {
      LCD_Rect_Fill(x - a_, y - b_, 2 * a_ + 1, 2 * b_ + 1, color24);
      LCD_Rect_Fill(x - b_, y - a_, 2 * b_ + 1, 2 * a_ + 1, color24);
    }
    else
    {
      LCD_Rect_Fill(a_ + x, b_ + y, size, size, color24);
      LCD_Rect_Fill(b_ + x, a_ + y, size, size, color24);
      LCD_Rect_Fill(x - a_, b_ + y, size, size, color24);
      LCD_Rect_Fill(x - b_, a_ + y, size, size, color24);
      LCD_Rect_Fill(b_ + x, y - a_, size, size, color24);
      LCD_Rect_Fill(a_ + x, y - b_, size, size, color24);
      LCD_Rect_Fill(x - a_, y - b_, size, size, color24);
      LCD_Rect_Fill(x - b_, y - a_, size, size, color24);
    }
    if (P < 0)
    {
      P = (P + 3) + (2 * a_);
      a_++;
    }
    else
    {
      P = (P + 5) + (2 * (a_ - b_));
      a_++;
      b_--;
    }
  }
}

void LCD_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint32_t color24)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
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
    if (cornername & 0x4) 
    {
      LCD_Rect_Fill(x0 + x, y0 + y, size, size, color24);
      LCD_Rect_Fill(x0 + y, y0 + x, size, size, color24);
    }
    if (cornername & 0x2) 
    {
      LCD_Rect_Fill(x0 + x, y0 - y, size, size, color24);
      LCD_Rect_Fill(x0 + y, y0 - x, size, size, color24);
    }
    if (cornername & 0x8) 
    {
      LCD_Rect_Fill(x0 - y, y0 + x, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 + y, size, size, color24);
    }
    if (cornername & 0x1) 
    {
      LCD_Rect_Fill(x0 - y, y0 - x, size, size, color24);
      LCD_Rect_Fill(x0 - x, y0 - y, size, size, color24);
    }
  }
}

void LCD_Rect_Round(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint32_t color24)
{
  LCD_Line(x + r, y, x + length + size - r, y, size, color24);
  LCD_Line(x + r, y + width - 1, x + length + size - r, y + width - 1, size, color24);
  LCD_Line(x, y + r, x, y + width - size - r, size, color24);
  LCD_Line(x + length - 1, y + r, x + length - 1, y + width - size - r, size, color24);
  LCD_Circle_Helper(x + r, y + r, r, 1, size, color24);
  LCD_Circle_Helper(x + length - r - 1, y + r, r, 2, size, color24);
  LCD_Circle_Helper(x + length - r - 1, y + width - r - 1, r, 4, size, color24);
  LCD_Circle_Helper(x + r, y + width - r - 1, r, 8, size, color24);
}

void LCD_Circle_Fill_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color24)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
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
    if (cornername & 0x1) 
    {
      LCD_Line(x0 + x, y0 - y, x0 + x, y0 - y + 2 * y + delta, 1, color24);
      LCD_Line(x0 + y, y0 - x, x0 + y, y0 - x + 2 * x + delta, 1, color24);
    }
    if (cornername & 0x2) 
    {
      LCD_Line(x0 - x, y0 - y, x0 - x, y0 - y + 2 * y + delta, 1, color24);
      LCD_Line(x0 - y, y0 - x, x0 - y, y0 - x + 2 * x + delta, 1, color24);
    }
  }
}

void LCD_Rect_Round_Fill(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint32_t color24)
{
  LCD_Rect_Fill(x + r, y, length - 2 * r, width, color24);
  LCD_Circle_Fill_Helper(x + length - r - 1, y + r, r, 1, width - 2 * r - 1, color24);
  LCD_Circle_Fill_Helper(x + r, y + r, r, 2, width - 2 * r - 1, color24);
}

static void LCD_Char(int16_t x, int16_t y, const GFXglyph *glyph, const GFXfont *font, uint8_t size, uint32_t color24)
{
  uint8_t *bitmap = font -> bitmap;
  uint16_t bo = glyph -> bitmapOffset;
  uint8_t bits = 0, bit = 0;
  uint16_t set_pixels = 0;
  uint8_t cur_x, cur_y;
  for(cur_y = 0; cur_y < glyph -> height; cur_y++)
  {
    for(cur_x = 0; cur_x < glyph -> width; cur_x++)
    {
      if(bit == 0)
      {
        bits = (*(const unsigned char *)(&bitmap[bo++]));
        bit = 0x80;
      }
      if(bits & bit)
      {
        set_pixels++;
      }
      else if (set_pixels > 0)
      {
        LCD_Rect_Fill(x + (glyph -> xOffset + cur_x - set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color24);
        set_pixels = 0;
      }
      bit >>= 1;
    }
    if (set_pixels > 0)
    {
      LCD_Rect_Fill(x + (glyph -> xOffset + cur_x-set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color24);
      set_pixels = 0;
    }
  }
}

void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font, uint8_t size, uint32_t color24)
{
  int16_t cursor_x = x;
  int16_t cursor_y = y;
  GFXfont font;
  memcpy((&font), (p_font), (sizeof(GFXfont)));
  for(uint16_t text_pos = 0; text_pos < strlen(text); text_pos++)
  {
    char c = text[text_pos];
    if(c == '\n')
    {
      cursor_x = x;
      cursor_y += font.yAdvance * size;
    }
    else if(c >= font.first && c <= font.last && c != '\r')
    {
      GFXglyph glyph;
      memcpy((&glyph), (&font.glyph[c - font.first]), (sizeof(GFXglyph)));
      LCD_Char(cursor_x, cursor_y, &glyph, &font, size, color24);
      cursor_x += glyph.xAdvance * size;
    }
  }
}

void LCD_Rotate(uint16_t rotate)
{
  LCD_Send_Cmd(0x36); // set address_mode
  LCD_Send_Dat(rotate);
}

void LCD_Init(void)
{
  // 1. Power up the system platform and assert the RESET# signal (‘L’ state) for a minimum of 100us to reset the controller.
  pin_high(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay (100);
  pin_low(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay (120);
  pin_high(FCMC_RESET_GPIO_Port,FCMC_RESET_Pin);
  HAL_Delay(150);
  /* 2. Configure SSD1961’s PLL frequency
  VCO = Input clock x (M + 1)
  PLL frequency = VCO / (N + 1)
  * Note :
  1. 250MHz < VCO < 800MHz
  PLL frequency < 110MHz
  2. For a 10MHz input clock to obtain 100MHz PLL frequency, user cannot program M = 19 and N = 1. The
  closet setting in this situation is setting M=29 and N=2, where 10 x 30 / 3 = 100MHz.
  3. Before PLL is locked, SSD1961/2/3 is operating at input clock frequency (e.g. 10MHz), registers
  programming cannot be set faster than half of the input clock frequency (5M words/s in this example).
  Example to program SSD1961 with M = 29, N = 2, VCO = 10M x 30 = 300 MHz, PLL frequency = 300M / 3 = 100
  MHz
  ******************************/

  LCD_Send_Cmd(LCD_RESET);
  LCD_Send_Cmd(LCD_DISPLAY_ON);   
  LCD_Send_Cmd(0xE2); // set frequency
  LCD_Send_Dat(0x1D); // presceller(M=29)
  LCD_Send_Dat(0x02); // multiplier(N=2)
  LCD_Send_Dat(0xFF); // on-off multiplier and presceller
  // 3. Turn on the PLL
  LCD_Send_Cmd(0xE0);
  LCD_Send_Dat(0x01);
  HAL_Delay(120); // Wait for 100us to let the PLL stable and read the PLL lock status bit.
  LCD_Send_Cmd(0xE0);
  // READ COMMAND “0xE4); (Bit 2 = 1 if PLL locked)
  LCD_Send_Dat(0x03); // 5. Switch the clock source to PLL
  HAL_Delay(120);
  LCD_Send_Cmd(0x01); // 6. Software Reset
  HAL_Delay(120);
  /*************
  Dot clock Freq = PLL Freq x (LCDC_FPR + 1) / 2^20
  For example, 22MHz = 100MHz * (LCDC_FPR+1) / 2^20
  LCDC_FPR = 230685 = 0x3851D
  ********************/
  LCD_Send_Cmd(0xE6); // 7. Configure the dot clock frequency
  LCD_Send_Dat(0x03);
  LCD_Send_Dat(0x85);
  LCD_Send_Dat(0x1D);
  // 8. Configure the LCD panel
  // a. Set the panel size to 480 x 800 and polarity of LSHIFT, LLINE and LFRAME to active low
  LCD_Send_Cmd(0xB0);
  if (LSHIFT) LCD_Send_Dat(0x0C); /* 0x08 0x0C 0xAE(5') */else LCD_Send_Dat(0xAE); // 18bit panel, disable dithering, LSHIFT: Data latch in rising edge, LLINE and LFRAME: active low
  LCD_Send_Dat(0x20); /* 0x00 0x80 0x20(5') */ // TFT type
  LCD_Send_Dat(0x03); // Horizontal Width: 480 - 1 = 0x031F
  LCD_Send_Dat(0x1F);
  LCD_Send_Dat(0x01); // Vertical Width : 800 -1 = 0x01DF
  LCD_Send_Dat(0xDF);
  LCD_Send_Dat(0x2D); /* 0x00 0x2d */ // 000 = режим RGB
  // b. Set the horizontal period
  LCD_Send_Cmd(0xB4); // Horizontal Display Period
  LCD_Send_Dat(0x03); // HT: horizontal total period (display + non-display) – 1 = 520-1 = 519 =0x0207
  LCD_Send_Dat(0xA0);
  LCD_Send_Dat(0x00); // HPS: Horizontal Sync Pulse Start Position = Horizontal Pulse Width + Horizontal Back Porch = 16 = 0x10
  LCD_Send_Dat(0x2E);
  LCD_Send_Dat(0x30); // HPW: Horizontal Sync Pulse Width - 1=8-1=7
  LCD_Send_Dat(0x00); // LPS: Horizontal Display Period Start Position = 0x0000
  LCD_Send_Dat(0x0F);
  LCD_Send_Dat(0x00); // LPSPP: Horizontal Sync Pulse Subpixel Start Position(for serial TFT interface). Dummy value for TFT interface.
  // c. Set the vertical period
  LCD_Send_Cmd(0xB6); // Vertical Display Period
  LCD_Send_Dat(0x02); // VT: Vertical Total (display + non-display) Period – 1 =647=0x287
  LCD_Send_Dat(0x0D);
  LCD_Send_Dat(0x00); // VPS: Vertical Sync Pulse Start Position = Vertical Pulse Width + Vertical Back Porch = 2+2=4
  LCD_Send_Dat(0x10);
  LCD_Send_Dat(0x10); // VPW: Vertical Sync Pulse Width – 1 =1
  LCD_Send_Dat(0x08); // FPS: Vertical Display Period Start Position = 0
  LCD_Send_Dat(0x00);
  // 9. Set the back light control PWM clock frequency
  // PWM signal frequency = PLL clock / (256 * (PWMF[7:0] + 1)) / 256
  LCD_Send_Cmd(0xBE); // PWM configuration
  LCD_Send_Dat(0x08); // set PWM signal frequency to 170Hz when PLL frequency is 100MHz
  LCD_Send_Dat(0xFF); // PWM duty cycle (50%)
  LCD_Send_Dat(0x01); // 0x09 = enable DBC, 0x01 = disable DBC // on
  
  LCD_Send_Cmd(0x36); // set address_mode
  LCD_Send_Dat(MIRROR);

  // 13. Setup the MCU interface for 16-bit data write (565 RGB)
  LCD_Send_Cmd(0xF0); // mcu interface config
  LCD_Send_Dat(0x03); // 16 bit interface (565)
  // 10. Turn on the display       
  LCD_Send_Cmd(LCD_DISPLAY_ON); // display on
}

void LCD_Bright(uint8_t bright)
{
  LCD_Send_Cmd(0xBE); // PWM configuration
  LCD_Send_Dat(0x08); // set PWM signal frequency to 170Hz when PLL frequency is 100MHz
  LCD_Send_Dat(bright); // PWM duty cycle
  LCD_Send_Dat(0x01);
}

