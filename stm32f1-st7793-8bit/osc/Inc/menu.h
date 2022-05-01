#ifndef __MAIN_H
#define __MAIN_H

#include "stdint.h"
#include "stdbool.h"

extern bool oscil_stop;
extern uint16_t buffering;

uint16_t buffer_flag = 0;
uint16_t trigger_flag_hi = 0;
uint16_t trigger_flag_lo = 0;
uint16_t scroll_flag_x = 0;
uint16_t scroll_flag_y = 0;
uint16_t speed_flag = 0;
uint16_t gain_flag = 0;
uint16_t chanal_flag = 0;

extern uint16_t buff_scroll_x;
extern int16_t buff_scroll_y;

// extern bool chanal_a;
// extern bool chanal_b;
// extern uint16_t chanal_flag;
extern uint16_t chanal;

extern uint16_t trigger;
extern uint32_t adc_speed;

extern uint16_t trigger_polar;

extern uint16_t gain;

char lb_buffer[8] = "buff";
char lb_scrool_y[8] = "setY";
char lb_scrool_x[8] = "setX";
char lb_speed[8] = "time";
char lb_positive[8] = "TriH";
char lb_negative[8] = "TriL";
char lb_gain[8] = "gain";
char lb_chanal[8] = "chan";

void write_str(uint16_t x, uint16_t y, int8_t* label);
void write_enc_val(uint16_t x, uint16_t y, int32_t value);
void write_val(uint16_t x, uint16_t y, int32_t value);
void write_val_mark(uint16_t x, uint16_t y, uint32_t size);
void write_marker_menu_position(uint16_t x, uint16_t y, uint16_t size);
void write_marker_rect(uint16_t x, uint16_t y, uint16_t size, uint16_t color);
void draw_menu_bar(void);
void menu_upgrade(void);

#endif /* __MAIN_H */

