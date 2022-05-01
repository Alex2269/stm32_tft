#ifndef SSD1963_H_
#define SSD1963_H_

#include "fonts_all_includes.h"
#include "color.h"

/* Set here your display resolution. */
extern uint16_t DISPLAY_WIDTH ; // = 800;
extern uint16_t DISPLAY_HEIGHT ; // = 480;

static inline uint32_t
convertRGBto888(uint8_t r, uint8_t g, uint8_t b)
{
 return (r << 16) | (g << 8) | b;
}

/* *** Function prototypes. *** */
uint16_t Display_Get_WIDTH(void);
uint16_t Display_Get_HEIGHT(void);
void Display_Set_WIDTH(uint16_t x);
void Display_Set_HEIGHT(uint16_t y);
void Display_Set_rotation(uint8_t r);
void Display_Init(uint16_t r);
void Display_Set_backlight(uint8_t percent);
void Display_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void Display_PSet(uint16_t x, uint16_t y, uint32_t c);

void DMA_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void DMA_Pixel_Set(uint16_t x, uint16_t y, uint32_t c);
uint32_t DMA_FillFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t c);

#endif /* SSD1963_H */

/* [] END OF FILE */

