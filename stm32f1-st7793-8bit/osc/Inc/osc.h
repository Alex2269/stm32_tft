#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -------*/
#include <string.h> //
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "tft_base.h"
//#include "tft_adafruit.h"
#include "tft_gfx.h"
#include "tft_gfx2.h"
#include "utoa.h"

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

void oscil_init(void);
void trigger_on(uint16_t trig);
void trigger_off(void);
void run_buffering(void);
void run_no_buffering(void);
void oscil_run(void);
void fill_buff(uint16_t i);
void rotate_buff(uint16_t r);
void clean_ray(void);
void flush_buff(void);
void graph_clean(void);
void graph_draw(void);
void graph_point_clean_a(uint16_t i);
void graph_point_clean_b(uint16_t i);
void graph_point_draw_a(uint16_t i);
void graph_point_draw_b(uint16_t i);
void graph_ray_clean_a(uint16_t i);
void graph_ray_clean_b(uint16_t i);
void graph_ray_draw_a(uint16_t i);
void graph_ray_draw_b(uint16_t i);
void write_graph_a(uint16_t i);
void write_graph_b(uint16_t i);
void write_grid(void);

void draw_menu_bar(void);
void menu_upgrade(void);

#ifdef __cplusplus
}
#endif

#endif /*__ osc_H */
