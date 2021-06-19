/*
 * 
 * ugui_ILI9325.h
 * 
 */

#ifndef UGUI_ILI9325_H_
#define UGUI_ILI9325_H_

#include "ugui.h"
#include "stdint.h"
#include "stdbool.h"

/* *** Configuration. *** */

    /* Set here your display resolution. */
#define DISPLAY_WIDTH           320
#define DISPLAY_HEIGHT          240

#define CONCATENATE(name, function)                 CONCAT(name, function)
#define CONCAT(name, function)                      name##function

/* *** Function prototypes. *** */
uint16_t get_width(void);
uint16_t get_height(void);
void set_width(uint16_t width);
void set_height(uint16_t height);
void Display_WriteCommand(uint8_t command);
void Display_WriteData(uint8_t data);
void Display_WriteMultiData(uint8_t *data, uint16_t size);
void Display_Reset();
void ILI9325_8bit(void);
void Display_Init();
void tft_SetRotation(uint8_t r);
void vertScroll(uint16_t top, uint16_t scrollines, uint16_t offset);
void invertDisplay(bool i);
void Display_WindowSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c);

    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize);

/* [] END OF FILE */


#endif /* UGUI_ILI9325_H_ */

