#ifndef TFT_gfx2_H
#define TFT_gfx2_H

/************************ TFT gfx2 LIBRARY ***************************
******** Description : This is library for TFT using STM32F103C8T6 uC ****
**************************************************************************/

/* Include Library */
#include "stdint.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "pins_ext.h"

/* Define for connection */

#include "../pgmspace.h"
#include "tft_defines.h"
#include "tft_base.h"
#include "gfxfont.h"

//-------------end--------------------------------

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

/* Declare Function Prototype */

/******************************** User Function ***********************************/

void fastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


#endif // TFT_gfx2_H
