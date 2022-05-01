#ifndef UGUI_SSD1963_H
#define UGUI_SSD1963_H

#include "ugui.h"
    
extern uint16_t DISPLAY_WIDTH ; // = 800;
extern uint16_t DISPLAY_HEIGHT ; // = 480;

/* *** Function prototypes. *** */
uint16_t Display_Get_WIDTH(void);
uint16_t Display_Get_HEIGHT(void);
void Display_Set_WIDTH(uint16_t x);
void Display_Set_HEIGHT(uint16_t y);
void Display_Set_rotation(uint8_t r);
void Display_Init(uint16_t r);
void Display_Set_backlight(uint8_t percent);
void Display_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c);

    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine(UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint8_t *image, uint16_t pSize);

#endif /* UGUI_SSD1963_H */

/* [] END OF FILE */
