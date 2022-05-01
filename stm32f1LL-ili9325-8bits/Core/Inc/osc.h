#ifndef __gpio_H
#define __gpio_H

/* Includes -------*/
#include "stdint.h"

#include "system_stm32f1xx.h"
#include "adc_cfg.h"
// #include "adc.h"
#include "tim.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "tft_base.h"
#include "tft_io.h"
#include "utoa.h"

void oscil_init(void);
void oscil_run(void);
void write_grid(void);
void write_text(uint16_t i);
void fill_buff(uint16_t i);
void rotate_buff(uint16_t r);
void write_graph(uint16_t i);

#ifdef __cplusplus
}
#endif
#endif /*__ osc_H */
