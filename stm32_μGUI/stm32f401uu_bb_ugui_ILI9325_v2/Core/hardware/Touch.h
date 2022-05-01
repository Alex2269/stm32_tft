/*
 * Touch.h
 *
 *  Created on: 28.10.2018
 *      Author: Harebit
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include "main.h"
#include "adc_cfg.h"

#ifdef __cplusplus
 extern "C" {
#endif

// Physical display size -> the same as in LCDConf.c

#define XSIZE_PHYS  320 // To be adapted to x-screen size
#define YSIZE_PHYS  240 // To be adapted to y-screen size
#define MIN_PRESSURE 100 // To be adapted to touch screen
#define MAX_PRESSURE 4000 // To be adapted to touch screen

volatile static int16_t pressure;

#define TOUCH_AD_LEFT 3400
#define TOUCH_AD_RIGHT 400
#define TOUCH_AD_TOP 3550
#define TOUCH_AD_BOTTOM 610

// #define XP_Pin GPIO_PIN_10  //PB10
// #define XP_GPIO_Port GPIOB
// #define YM_Pin GPIO_PIN_8	//PA8
// #define YM_GPIO_Port GPIOA
// #define YP_Pin GPIO_PIN_1	//PA1
// #define YP_GPIO_Port GPIOA
// #define XM_Pin GPIO_PIN_4	//PA4
// #define XM_GPIO_Port GPIOA

// probable touchscreen is and on other contacts
#define YM_Pin LCD_D7_Pin // Y(-)
#define YM_GPIO_Port LCD_D7_GPIO_Port
#define XP_Pin LCD_D6_Pin // X(+)
#define XP_GPIO_Port LCD_D6_GPIO_Port

#define YP_Pin LCD_WR_Pin // Y(+)
#define YP_GPIO_Port LCD_WR_GPIO_Port
#define XM_Pin LCD_DC_Pin // X(-)
#define XM_GPIO_Port LCD_DC_GPIO_Port

#define MODE_INPUT                 (0U) /*!< Select input mode */
#define MODE_OUTPUT                0x1UL  /*!< Select output mode */
#define MODE_ALTERNATE             0x2UL  /*!< Select alternate function mode */
#define MODE_ANALOG                0x3UL    /*!< Select analog mode */

uint16_t ugui_TOUCH_MeasureX(void);
uint16_t ugui_TOUCH_MeasureY(void);
int16_t ugui_TOUCH_MeasureZ(void);
int16_t ugui_TOUCH_Px(void);
int16_t ugui_TOUCH_Py(void);
uint8_t ugui_TOUCH_StateZ(void);
void GPIO_X_MeasurementSetup(void);
uint16_t GPIO_X_Measurement(void);
void GPIO_Y_MeasurementSetup(void);
uint16_t GPIO_Y_Measurement(void);
void GPIO_Z_MeasurementSetup(void);
int16_t GPIO_Z_Measurement(void);
uint16_t GPIO_Measurement(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIOx, uint32_t Pin);
void GPIO_Restore_Outputs(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* TOUCH_H_ */

