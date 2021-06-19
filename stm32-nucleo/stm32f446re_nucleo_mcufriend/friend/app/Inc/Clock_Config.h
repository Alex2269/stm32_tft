#ifndef __Clock_Config_H__
#define __Clock_Config_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void SystemClock_Config(void);
void Error_Handler(void);

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /*__Clock_Config_H__ */
