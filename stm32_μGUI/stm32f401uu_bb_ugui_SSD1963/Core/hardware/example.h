/*
 * 
 * example.h
 *
 */


#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <ugui_SSD1963.h>
#include "main.h"
//#include "stm32f1xx_hal.h"
#include "ugui.h"
#include "Touch.h"

#define MAX_OBJECTS 10
#define BuffOUT     40

void window_1_callback(UG_MESSAGE *msg);

void begin (void);
void loop (void);


#endif /* EXAMPLE_H_ */
