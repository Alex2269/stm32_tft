#ifndef _adc_cfg_H
#define _adc_cfg_H
// file: adc_cfg.h

#include <stdint.h>
#include "main.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"

void MX_ADC1_Init(void); // copy from adc.h
void MX_ADC2_Init(void); // copy from adc.h

void ADC_Init(ADC_TypeDef *ADCx);

uint16_t adc_get_value(ADC_TypeDef *ADCx, uint8_t channel);
uint16_t read_adc(ADC_TypeDef *ADCx, uint8_t channel);

#endif // _adc_cfg_H
