/*
 * 
 * ugui_R61581.h
 * 
 */

#ifndef UGUI_R61581_H_
#define UGUI_R61581_H_

#include "ugui.h"
#include "stdint.h"
#include "stdbool.h"


/* *** Configuration. *** */

    /* Set here your display resolution. */
// #define DISPLAY_WIDTH           320
// #define DISPLAY_HEIGHT          480
extern uint16_t DISPLAY_WIDTH ;
extern uint16_t DISPLAY_HEIGHT ;

#define CONCATENATE(name, function)                 CONCAT(name, function)
#define CONCAT(name, function)                      name##function

/* *** Function prototypes. *** */

void Display_Reset(void);
void R61581_8bit(void);
void Display_Init(void);
void Display_WindowSet(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c);

    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint16_t *image, uint16_t pSize);

/* [] END OF FILE */


#endif /* UGUI_R61581_H_ */

