#ifndef TOUCH_H_
#define TOUCH_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "adc_cfg.h"

// probable touchscreen is and on other contacts
#define YM_Pin LCD_D0_Pin   // PB0
#define YM_GPIO_Port LCD_D0_GPIO_Port
#define XP_Pin LCD_D1_Pin   // PB1
#define XP_GPIO_Port LCD_D1_GPIO_Port

#define YP_Pin LCD_WR_Pin   // PB12
#define YP_GPIO_Port LCD_WR_GPIO_Port
#define XM_Pin LCD_DC_Pin   // PB13
#define XM_GPIO_Port LCD_DC_GPIO_Port

uint16_t ugui_TOUCH_MeasureX(void);
uint16_t ugui_TOUCH_MeasureY(void);
void gpio_x_connection(void);
void gpio_y_connection(void);
uint16_t GPIO_Measurement(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIOx, uint32_t Pin);
void GPIO_Restore_Outputs(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* TOUCH_H_ */

