#include "stdbool.h"
#include "stdint.h"

#include "stm32f1xx_hal.h"
#include "main.h"

// Color definitions
#define COLOR_BLACK       0x0000      /*   0,   0,   0 */
#define COLOR_NAVY        0x000F      /*   0,   0, 128 */
#define COLOR_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define COLOR_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define COLOR_MAROON      0x7800      /* 128,   0,   0 */
#define COLOR_PURPLE      0x780F      /* 128,   0, 128 */
#define COLOR_OLIVE       0x7BE0      /* 128, 128,   0 */
#define COLOR_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define COLOR_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define COLOR_BLUE        0x001F      /*   0,   0, 255 */
#define COLOR_GREEN       0x07E0      /*   0, 255,   0 */
#define COLOR_CYAN        0x07FF      /*   0, 255, 255 */
#define COLOR_RED         0xF800      /* 255,   0,   0 */
#define COLOR_MAGENTA     0xF81F      /* 255,   0, 255 */
#define COLOR_YELLOW      0xFFE0      /* 255, 255,   0 */
#define COLOR_WHITE       0xFFFF      /* 255, 255, 255 */
#define COLOR_ORANGE      0xFD20      /* 255, 165,   0 */
#define COLOR_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define COLOR_PINK        0xF81F

#define SetCursorPos(X,Y) WriteRegister(0x20); \
                       WriteData(0, Y); \
                       WriteRegister(0x21); \
                       WriteData(0, X)

#define WriteColor(Color) WriteData(Color>>8, Color&0x0ff)

void WriteRegister(uint8_t IndexRegister);
// void WriteColor(uint16_t Color);
void WriteData(uint8_t HiData, uint8_t LoData);
void DrawPixel(uint8_t X, uint8_t Y, uint16_t Color);
void DrawHLine(uint8_t X, uint8_t Y, uint8_t Width, uint16_t Color);
void DrawShape8x8(const uint8_t X, uint8_t Y, const uint8_t Scale, const uint8_t* Shape, const uint8_t Palette, const bool FlipVertical, const bool FlipHorizontal);
void DrawShape16x16(const uint8_t X, uint8_t Y, const uint8_t Scale, const uint8_t* ShapeTopLeft, const uint8_t* ShapeTopRight, const uint8_t* ShapeBottomLeft, const uint8_t* ShapeBottomRight, const uint8_t Palette);
void ClearDisplay();
void DrawArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint16_t Color);
void ScrollLeft();
void ScrollRight();
void InitDisplay();
void fillCircle(int x, int y, int radius, uint16_t Color);
