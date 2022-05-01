#ifndef _TIME_H
#define _TIME_H

#include "FreeRTOS.h"
#include "stdbool.h"

void delay_nop(uint32_t volatile nano);
void delay_us(uint32_t volatile us);
void delay_ms(uint32_t volatile ms);
uint32_t get_time_reload(void);
uint32_t get_time_current(void);
uint32_t get_time_us(void);
uint32_t get_time_ms(void);
uint32_t get_time_sec(void);

#endif // _TIME_H




