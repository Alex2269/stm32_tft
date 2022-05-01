/*
 * Touch.h
 *
 *  Created on: 28.10.2018
 *      Author: Harebit
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "adc_cfg.h"

// Physical display size -> the same as in LCDConf.c

extern uint16_t DISPLAY_WIDTH; // To be adapted to x-screen size
extern uint16_t DISPLAY_HEIGHT; // To be adapted to y-screen size

#define MIN_PRESSURE 100 // To be adapted to touch screen

volatile static int16_t pressure;

#define TOUCH_AD_LEFT 3400
#define TOUCH_AD_RIGHT 400
#define TOUCH_AD_TOP 3550
#define TOUCH_AD_BOTTOM 610

#define XP_Pin LL_GPIO_PIN_1  // PB10
#define XP_GPIO_Port GPIOA
#define YM_Pin LL_GPIO_PIN_2   // PA8
#define YM_GPIO_Port GPIOA
#define YP_Pin LL_GPIO_PIN_3   // PA1
#define YP_GPIO_Port GPIOA
#define XM_Pin LL_GPIO_PIN_4   // PA4
#define XM_GPIO_Port GPIOA

void Init_ADC_Pins(void);
uint16_t ugui_TOUCH_MeasureX(void);
uint16_t ugui_TOUCH_MeasureY(void);
int16_t ugui_TOUCH_MeasureZ(void);
uint8_t ugui_TOUCH_StateZ(void);
int16_t ugui_TOUCH_Px(void);
void GPIO_X_MeasurementSetup(void);
void GPIO_Y_MeasurementSetup(void);
void GPIO_Z_MeasurementSetup(void);
uint16_t GPIO_Measurement(ADC_TypeDef *ADCx, uint8_t channel);
void GPIO_Restore_Outputs(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* TOUCH_H_ */
