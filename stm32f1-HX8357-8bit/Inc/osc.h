#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -------*/
#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "tft_base.h"
#include "utoa.h"

void oscil_init(void);
void trigger_on(uint16_t trig);
void trigger_off(void);
void oscil_run(void);
void fill_buff(uint16_t i);
void rotate_buff(uint16_t r);
void clean_ray(void);
void flush_buff(void);
void write_marker_point(uint16_t x, uint16_t y, uint16_t size);
void write_text(uint16_t i);
void write_string(uint16_t x, uint16_t y, int8_t* label);
void write_value(uint16_t x, uint16_t y, uint16_t value);
void write_graph(uint16_t i);
void write_grid(void);

#ifdef __cplusplus
}
#endif

#endif /*__ osc_H */
