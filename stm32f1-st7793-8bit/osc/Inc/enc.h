/**
  * @brief  encoder connected to pins: PB3 and PB4
  * @param  see source file: enc.c
  * @retval None
***/

#ifndef __encoder_H
#define __encoder_H

#include "pins_ext.h"

extern bool oscil_stop;
extern uint16_t buffering;

extern bool chanal_a;
extern bool chanal_b;
extern uint16_t chanal_flag;
uint16_t chanal;

extern uint16_t buffer_flag;
uint16_t gain;
extern uint16_t gain_flag;
extern uint16_t buff_scroll_x;
extern int16_t buff_scroll_y;

extern uint16_t trigger;
extern uint32_t adc_speed;
uint32_t spped_point;

extern uint16_t trigger_flag_hi;
extern uint16_t trigger_flag_lo;

extern uint16_t scroll_flag_x;
extern uint16_t scroll_flag_y;
extern uint16_t speed_flag;
extern uint16_t trigger_polar;

void delay_enc(uint32_t tics);
void write_enc_val(uint16_t x, uint16_t y, int32_t value);
void write_marker_rect(uint16_t x, uint16_t y, uint16_t size, uint16_t color);
void write_marker_menu_position(uint16_t x, uint16_t y, uint16_t size);
void write_str(uint16_t x, uint16_t y, int8_t* label);
void write_val_mark(uint16_t x, uint16_t y, uint16_t size);
void buttons(void);
void encoder_rising(void);
void encoder_faling(void);

void encoder_init(void);
void encoder_re_init(void);

#endif /* __encoder_H */

