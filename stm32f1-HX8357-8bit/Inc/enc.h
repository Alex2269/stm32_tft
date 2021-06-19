/**
  * @brief  encoder connected to pins: PB3 and PB4
  * @param  see source file: enc.c
  * @retval None
***/

#ifndef __encoder_H
#define __encoder_H

#include "pins_ext.h"

void buttons(void);
void encoder_rising(void);
void encoder_faling(void);

#endif /* __encoder_H */
