#include <math.h>
#include "init_tft.h"
#include "tft_base.h"
#include "tft_font.h"
#include "tft_io.h"

#define DOT_SIZE 8

uint16_t _model;
uint16_t _width;
uint16_t _height;
uint8_t _rotation;

#define swap16(a, b) { uint16_t t = a; a = b; b = t; }

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

/* Declare Private Variable */
// static inline
void delay_us(uint32_t volatile microsec)
{
  microsec *=(SystemCoreClock/1000000)/5;
  while(microsec--);
}

// static inline
void delay(uint32_t volatile msec)
{
  msec *=(SystemCoreClock/1000)/5;
  while(msec--);
}

GPIO_TypeDef* PORT_NAME_ARR[]=
{
  // D0_PORT,D1_PORT,D2_PORT,D3_PORT,D4_PORT,D5_PORT,D6_PORT,D7_PORT,
  // D8_PORT,D9_PORT,D10_PORT,D11_PORT,D12_PORT,D13_PORT,D14_PORT,D15_PORT
};

uint16_t PIN_NAME_ARR[]=
{
  // D0_PIN,D1_PIN,D2_PIN,D3_PIN,D4_PIN,D5_PIN,D6_PIN,D7_PIN,
  // D8_PIN,D9_PIN,D10_PIN,D11_PIN,D12_PIN,D13_PIN,D14_PIN,D15_PIN
};

uint16_t BACK_COLOR,POINT_COLOR;

void lcd_model(uint16_t model, uint16_t width, uint16_t height)
{
  _model = model;
  set_width(width);
  set_height(height);
}

void invertDisplay(bool i)
{
  if(i) tft_write_reg16(0x21);
  else tft_write_reg16(0x20);
  tft_write_reg16_data16(0x61, 1);
}

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

// --
void lcdSetup(void)
{
    // init_table16(regValues_2, sizeof(regValues_2)); // flip
    ili9325_init();
    // init_table(reset_off, sizeof(reset_off));
    // init_table(wake_on, sizeof(wake_on));
    tft_SetRotation(0); // PORTRAIT
    invertDisplay(0);
}

// --
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
  tft_address_set(0, 0, get_width() - 1, get_height() - 1);
  vertScroll(0, get_height(), 0); // reset scrolling after a rotation
}

void vertScroll(uint16_t top, uint16_t scrollines, uint16_t offset)
{
  int16_t vsp;
  if(offset <= -scrollines || offset >= scrollines) offset = 0; // valid scroll
  vsp = top + offset; // vertical start position
  if(offset < 0)  vsp += scrollines; // eep in unsigned range

  tft_write_reg16_data16(0x61, 3); // !NDL, VLE, REV
  tft_write_reg16_data16(0x6A, vsp); // VL#
}

/**
 * @brief Draw rectangule of filling
 * @param x1:Start X coordinate
 * @param y1:Start Y coordinate
 * @param x2:End X coordinate
 * @param y2:End Y coordinate
 * @param color: pixel color
 * @retval None
 */
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  int i,j;
  
  if(x1 >= get_width()) return;
  if(x1 < 0) x1=0;
  if(x2 < 0) return;
  if(x2 >= get_width()) x2= get_width()-1;
  if(y1 >= get_height()) return;
  if(y1 < 0) y1=0;
  if(y2 < 0) return;
  if(y2 >= get_height()) y2= get_height()-1;
  tft_address_set(x1, y1, x2, y2);
  
  for(j=y1;j<=y2;j++)
  {
    // tft_write_reg16_data16(0x0020, x1); // RAM Address Set 1
    // tft_write_reg16_data16(0x0021, j); // RAM Address Set 2
    for(i=x1;i<=x2;i++)
    {
      tft_write_data16(color); // Write Data to GRAM
    }
  }
}

// Display Off
void lcdDisplayOff(void)
{
  tft_write_reg16_data16(0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
}

// Display On
void lcdDisplayOn(void)
{
  tft_write_reg16_data16(0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
}

/**
 * @brief Fill screen
 * @param color: pixel color
 * @retval None
 */
void lcdFillScreen(uint16_t color)
{
  lcdDrawFillRect(0, 0, get_width()-1, get_height()-1, color);
}

/**
 * @brief Draw line
 * @param x1:Start X coordinate
 * @param y1:Start Y coordinate
 * @param x2:End X coordinate
 * @param y2:End Y coordinate
 * @param color: pixel color
 * @retval None
 */
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  int i;
  int dx,dy;
  int sx,sy;
  int E;
  
  /* distance between two points */
  dx = (x2 > x1) ? x2 - x1 : x1 - x2;
  dy = (y2 > y1) ? y2 - y1 : y1 - y2;
  /* direction of two point */
  sx = (x2 > x1) ? 1 : -1;
  sy = (y2 > y1) ? 1 : -1;
  
  /* inclination < 1 */
  if(dx > dy)
  {
    E = -dx;
    for(i = 0 ; i <= dx ; i++)
    {
      tft_draw_point(x1, y1, color);
      x1 += sx;
      E += 2 * dy;
      if(E >= 0)
      {
        y1 += sy;
        E -= 2 * dx;
      }
    }
    /* inclination >= 1 */
  }
  else
  {
    E = -dy;
    for(i = 0 ; i <= dy ; i++)
    {
      tft_draw_point(x1, y1, color);
      y1 += sy;
      E += 2 * dx;
      if(E >= 0)
      {
        x1 += sx;
        E -= 2 * dy;
      }
    }
  }
}

/**
 * @brief Draws an Circle on LCD.
 * @param x0:Central X coordinate
 * @param y0:Central Y coordinate
 * @param r:radius
 * @param color: pixel color
 * @retval None
 */
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  
  x=0;
  y=-r;
  err=2-2*r;
  
  do
  {
    tft_draw_point(x0-x,y0+y,color);
    tft_draw_point(x0-y,y0-x,color);
    tft_draw_point(x0+x,y0-y,color);
    tft_draw_point(x0+y,y0+x,color);
    if((old_err=err)<=x) err+=++x*2+1;
    if(old_err>y || err>x) err+=++y*2+1;
  }
  while(y<0);
}

/**
 * @brief Draws an Circle on filling LCD.
 * @param x0:Central X coordinate
 * @param y0:Central Y coordinate
 * @param r:radius
 * @param color: pixel color
 * @retval None
 */
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  int ChangeX;
  
  x=0;
  y=-r;
  err=2-2*r;
  ChangeX=1;
  
  do
  {
    if(ChangeX)
    {
      lcdDrawLine(x0-x,y0-y,x0-x,y0+y,color);
      lcdDrawLine(x0+x,y0-y,x0+x,y0+y,color);
    }
    // if
    ChangeX=(old_err=err)<=x;
    if(ChangeX) err+=++x*2+1;
    if(old_err>y || err>x) err+=++y*2+1;
  }
  while(y<=0);
}

/**
 * @brief Draws an ellipse on LCD.
 * @param Xpos: X position
 * @param Ypos: Y position
 * @param XRadius: Ellipse X radius
 * @param YRadius: Ellipse Y radius
 * @param color: pixel color
 * @retval None
 */
void lcdDrawEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius, uint16_t color)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  K = (float)(rad2/rad1);
  
  do 
  {
    tft_draw_point((Xpos-(uint16_t)(x/K)), (Ypos+y), color);
    tft_draw_point((Xpos+(uint16_t)(x/K)), (Ypos+y), color);
    tft_draw_point((Xpos+(uint16_t)(x/K)), (Ypos-y), color);
    tft_draw_point((Xpos-(uint16_t)(x/K)), (Ypos-y), color);
    e2 = err;
    if(e2 <= x) 
    {
      err += ++x*2+1;
      if(-y == x && e2 <= y) e2 = 0;
    }
    if(e2 > y) err += ++y*2+1;
  }
  while(y <= 0);
}

/**
 * @brief Draws a full ellipse.
 * @param Xpos: X position
 * @param Ypos: Y position
 * @param XRadius: Ellipse X radius
 * @param YRadius: Ellipse Y radius
 * @param color: color
 * @retval None
 */
void lcd_FillEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius, uint16_t color)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  K = (float)(rad2/rad1);
  
  do
  {
    lcd_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), color);
    lcd_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), color);
    e2 = err;
    if(e2 <= x)
    {
      err += ++x*2+1;
      if(-y == x && e2 <= y) e2 = 0;
    }
    if(e2 > y) err += ++y*2+1;
  }
  while(y <= 0);
}

/**
 * @brief Draws an horizontal line.
 * @param Xpos: X position
 * @param Ypos: Y position
 * @param Length: Line length
 * @param color: pixel color
 * @retval None
 */
void lcd_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t color)
{
  for(uint32_t index = 0; index < Length; index++)
  {
    tft_draw_point((Xpos + index), Ypos, color);
  }
}

/**
 * @brief Draws a vertical line.
 * @param Xpos: X position
 * @param Ypos: Y position
 * @param Length: Line length
 * @param color: pixel color
 * @retval None
 */
void lcd_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t color)
{
  for(uint32_t index = 0; index < Length; index++)
  {
    tft_draw_point(Xpos, Ypos + index, color);
  }
}


/**
 * @brief Draw rectangule with round corner
 * @param x1:Start X coordinate
 * @param y1:Start Y coordinate
 * @param x2:End X coordinate
 * @param y2:End Y coordinate
 * @param r:radius
 * @param color:color
 * @retval None
 */
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color)
{
  int x;
  int y;
  int err;
  int old_err;
  unsigned char temp;
  
  if(x1>x2)
  {
    temp=x1; x1=x2; x2=temp;
  }
  // if
  if(y1>y2)
  {
    temp=y1; y1=y2; y2=temp;
  }
  // if
  x=0;
  y=-r;
  err=2-2*r;
  
  do
  {
    if(x)
    {
      tft_draw_point(x1+r-x,y1+r+y,color);
      tft_draw_point(x2-r+x,y1+r+y,color);
      tft_draw_point(x1+r-x,y2-r-y,color);
      tft_draw_point(x2-r+x,y2-r-y,color);
    }
    // if
    if((old_err=err)<=x) err+=++x*2+1;
    if(old_err>y || err>x) err+=++y*2+1;
  }
  while(y<0);
  
  lcdDrawLine(x1+r,y1 ,x2-r,y1 ,color);
  lcdDrawLine(x1+r,y2 ,x2-r,y2 ,color);
  lcdDrawLine(x1 ,y1+r,x1 ,y2-r,color);
  lcdDrawLine(x2 ,y1+r,x2 ,y2-r,color);
}

/**
 * @brief Draw arrow
 * @param x1:Start X coordinate
 * @param y1:Start Y coordinate
 * @param x2:End X coordinate
 * @param y2:End Y coordinate
 * @param w:Width of the botom
 * @param color:color
 * @retval None
 */
void lcdDrawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color)
{
  double Vx= x1 - x0;
  double Vy= y1 - y0;
  double v = sqrt(Vx*Vx+Vy*Vy);
  // printf("v=%f\n",v);
  double Ux= Vx/v;
  double Uy= Vy/v;
  uint16_t L[2],R[2];
  
  L[0]= x1 - Uy*w - Ux*v;
  L[1]= y1 + Ux*w - Uy*v;
  R[0]= x1 + Uy*w - Ux*v;
  R[1]= y1 - Ux*w - Uy*v;
  // printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
  // lcdDrawLine(x0,y0,x1,y1,color);
  lcdDrawLine(x1,y1,L[0],L[1],color);
  lcdDrawLine(x1,y1,R[0],R[1],color);
  lcdDrawLine(L[0],L[1],R[0],R[1],color);
}

/**
 * @brief Draw arrow of filling
 * @param x1:Start X coordinate
 * @param y1:Start Y coordinate
 * @param x2:End X coordinate
 * @param y2:End Y coordinate
 * @param w:Width of the botom
 * @param color:color
 * @retval None
 */
void lcdDrawFillArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color)
{
  double Vx= x1 - x0;
  double Vy= y1 - y0;
  double v = sqrt(Vx*Vx+Vy*Vy);
  // printf("v=%f\n",v);
  double Ux= Vx/v;
  double Uy= Vy/v;
  uint16_t L[2],R[2];
  
  L[0]= x1 - Uy*w - Ux*v;
  L[1]= y1 + Ux*w - Uy*v;
  R[0]= x1 + Uy*w - Ux*v;
  R[1]= y1 - Ux*w - Uy*v;
  // printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
  lcdDrawLine(x0,y0,x1,y1,color);
  lcdDrawLine(x1,y1,L[0],L[1],color);
  lcdDrawLine(x1,y1,R[0],R[1],color);
  lcdDrawLine(L[0],L[1],R[0],R[1],color);
  
  int ww;
  
  for(ww=w-1;ww>0;ww--)
  {
    L[0]= x1 - Uy*ww - Ux*v;
    L[1]= y1 + Ux*ww - Uy*v;
    R[0]= x1 + Uy*ww - Ux*v;
    R[1]= y1 - Ux*ww - Uy*v;
    // printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
    lcdDrawLine(x1,y1,L[0],L[1],color);
    lcdDrawLine(x1,y1,R[0],R[1],color);
  }
}

/**
 * RGB565 conversion
 * RGB565 is R(5)+G(6)+B(5)=16bit color format.
 * Bit image "RRRRRGGGGGGBBBBB"
 */
uint16_t rgb565_conv(uint16_t r, uint16_t g, uint16_t b)
{
  unsigned int RR,GG,BB;
  
  RR = (r * 31 / 255) << 11;
  GG = (g * 63 / 255) << 5;
  BB = (b * 31 / 255);
  return(RR | GG | BB);
}

void tft_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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

void tft_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
  if(x < 0 || x >= get_width()) return;
  if(y < 0 || y >= get_height()) return;
  
  tft_address_set(x, y, x+1, y+1);
  tft_write_reg16(0x0022);
  
  int16_t i,j;
  
  for(i=x;i<x+1;i++)
  {
    for(j=y;j<y+1;j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_draw_point_big(uint16_t x, uint16_t y, uint16_t color)
{
  if(x < 0 || x >= get_width()) return;
  if(y < 0 || y >= get_height()) return;
  
  tft_address_set(x-1, y-1, x+1, y+1);
  tft_write_reg16(0x0022);
  
  int16_t i,j;
  
  for(i=x-1;i<=x+1;i++)
  {
    for(j=y-1;j<=y+1;j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_draw_point_big2(uint16_t x, uint16_t y, uint16_t color)
{
  if(x < 0 || x >= get_width()) return;
  if(y < 0 || y >= get_height()) return;
  
  int16_t i,j;
  
  tft_address_set(x-2, y-2, x+2, y+2);
  tft_write_reg16(0x0022);
  
  for(i=x-2;i<=x+2;i++)
  {
    for(j=y-2;j<=y+2;j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_clear(uint16_t color)
{
  uint16_t i,j;
  
  tft_address_set(0,0, get_width()-1, get_height()-1);
  tft_write_reg16(0x0022);
  
  for(i=0;i< get_width();i++)
  {
    for(j=0;j< get_height();j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_fill(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color)
{
  if(row1 < 0 || row2 >= get_width()) return;
  if(column1 < 0 || column2 >= get_height()) return;
  
  uint16_t i,j;
  
  tft_address_set(row1,column1, row2, column2);
  tft_write_reg16(0x0022);
  
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_fill_rect(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color)
{
  if(row1 < 0 || row2 >= get_width()) return;
  if(column1 < 0 || column2 >= get_height()) return;
  
  uint16_t i,j;
  
  tft_address_set(row1,column1, row2, column2);
  tft_write_reg16(0x0022);
  
  for(i=row1;i<=row2;i++)
  {
    for(j=column1;j<=column2;j++)
    {
      tft_write_color_fast(color);
    }
  }
}

void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color)
{
  if(row1 < 0 || row2 >= get_width()) return;
  if(column1 < 0 || column2 >= get_height()) return;
  
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
  
  if(delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  
  for(t=0;t<=distance+1;t++)
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

void tft_draw_dot_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2, uint16_t color)
{
  if(row1 < 0 || row2 >= get_width()) return;
  if(column1 < 0 || column2 >= get_height()) return;
  
  tft_address_set(row1,column1, row1, column1);
  tft_write_reg16(0x0022);
  
  uint16_t t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;
  
  delta_x=column2-column1;
  delta_y=row2-row1;
  uRow=column1;
  uCol=row1;
  
  if(delta_x>0)incx=DOT_SIZE;
  else if(delta_x==0)incx=0;
  else
  {
    incx=-DOT_SIZE;delta_x=-delta_x;
  }
  
  if(delta_y>0)incy=DOT_SIZE;
  else if(delta_y==0)incy=0;
  else
  {
    incy=-DOT_SIZE;delta_y=-delta_y;
  }
  
  if(delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  
  for(t=0;t<=distance+1;t+=DOT_SIZE)
  {
    tft_write_color_fast(color);
    
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

void tft_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  tft_draw_line(x1,y1,x1,y2,color);
  tft_draw_line(x1,y2,x2,y2,color);
  tft_draw_line(x2,y2,x2,y1,color);
  tft_draw_line(x2,y1,x1,y1,color);
}

void tft_draw_circle(uint16_t row, uint16_t column, uint16_t radian, uint16_t color)
{
  int a,b;
  int32_t di;
  
  a=0;b=radian;
  di=3-(radian<<1);
  
  while(a<=b)
  {
    tft_draw_point(column-b,row-a,color);
    tft_draw_point(column+b,row-a,color);
    tft_draw_point(column-a,row+b,color);
    tft_draw_point(column-b,row-a,color);
    tft_draw_point(column-a,row-b,color);
    tft_draw_point(column+b,row+a,color);
    tft_draw_point(column+a,row-b,color);
    tft_draw_point(column+a,row+b,color);
    tft_draw_point(column-b,row+a,color);
    a++;
    // resenham
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
  tft_write_reg16(0x0022);
  
  for(i=x;i<(size_x*size_y);i++)
  {
    low_byte=*(image_arr+i*2);
    high_byte=*(image_arr+i*2+1);
    temp=((high_byte)<<8|(low_byte));
    tft_write_data(temp>>8);
    tft_write_data(temp&0x00ff);
  }
}

void tft_puts_image(const unsigned char* image_arr)
{
  uint32_t i;
  uint16_t temp;
  uint8_t high_byte,low_byte;
  
  tft_address_set(0,0, get_width()-1, get_height()-1);
  tft_write_reg16(0x0022);
  
  for(i=0;i<(get_width()* get_height());i++)
  {
    low_byte=*(image_arr+i*2);
    high_byte=*(image_arr+i*2+1);
    temp=((high_byte)<<8|(low_byte));
    tft_write_data(temp>>8);
    tft_write_data(temp&0x00ff);
  }
}

void tft_putchar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  
  // int16_t x0=x;
  uint16_t colortemp=POINT_COLOR;
  
  if(x> get_width()-16 || y> get_height()-16)return;
  num=num-' ';
  
  // tft_address_set(x,y,x+8-1,y+16-1);
  // tft_write_reg16(0x0022);
  
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
        if(temp&0x01)tft_draw_point(x+t,y+pos,POINT_COLOR);
        temp>>=1;
      }
    }
  }
  POINT_COLOR=colortemp;
}

void tft_putchar14x24(uint16_t x, uint16_t y, uint8_t data, uint8_t mode)
{
  if((x > get_width()-14) || (y > get_height()-24)) return;
  
  uint8_t i,j,k,temp;
  
  tft_address_set(x,y,x+14-1,y+24-1);
  tft_write_reg16(0x0022);
  
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
            tft_write_color_fast(POINT_COLOR);
          }
          else
          {
            tft_write_color_fast(BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_write_color_fast(POINT_COLOR);
          }
        }
      }
    }
  }
}

void tft_putchar18x32(uint16_t x, uint16_t y, uint8_t data, uint8_t mode)
{
  if((x > get_width()-18) || (y > get_height()-32)) return;
  
  uint8_t i,j,k,temp;
  
  tft_address_set(x,y,x+18-1,y+32-1);
  tft_write_reg16(0x0022);
  
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
            tft_write_color_fast(POINT_COLOR);
          }
          else
          {
            tft_write_color_fast(BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_write_color_fast(POINT_COLOR);
          }
        }
      }
    }
  }
}

void tft_putchar26x48(uint16_t x, uint16_t y, uint8_t data, uint8_t mode)
{
  if((x > get_width()-26) || (y > get_height()-48)) return;
  
  uint8_t i,j,k,temp;
  
  tft_address_set(x,y,x+26-1,y+48-1);
  tft_write_reg16(0x0022);
  
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
            tft_write_color_fast(POINT_COLOR);
          }
          else
          {
            tft_write_color_fast(BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
            tft_write_color_fast(POINT_COLOR);
          }
        }
      }
    }
  }
}

void tft_puts26x48(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  
  while(*(string+i)!='\0')
  {
    tft_putchar26x48(column,row,*(string+i),TFT_STRING_MODE);
    column+=25;
    i++;
  }
}

void tft_puts18x32(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  
  while(*(string+i)!='\0')
  {
    tft_putchar18x32(column,row,*(string+i),TFT_STRING_MODE);
    column+=17;
    i++;
  }
}

void tft_puts14x24(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  
  while(*(string+i)!='\0')
  {
    tft_putchar14x24(column,row,*(string+i),TFT_STRING_MODE);
    column+=13;
    i++;
  }
}

void tft_puts8x16(uint16_t row, uint16_t column, int8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  
  while(*(string+i)!='\0')
  {
    if(column> get_width()-16)
    {
      column=0;row+=16;
    }
    if(row> get_height()-16)
    {
      row=column=0;
    }
    
    tft_putchar(column,row,*(string+i),TFT_STRING_MODE);
    column+=8;
    i++;
  }
}

