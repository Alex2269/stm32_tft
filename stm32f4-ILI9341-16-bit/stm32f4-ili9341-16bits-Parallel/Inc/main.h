/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define DSP_RST_Pin GPIO_PIN_5
#define DSP_RST_GPIO_Port GPIOC
#define DSP_LED_Pin GPIO_PIN_0
#define DSP_LED_GPIO_Port GPIOB
#define DSP_4_Pin GPIO_PIN_7
#define DSP_4_GPIO_Port GPIOE
#define DSP_5_Pin GPIO_PIN_8
#define DSP_5_GPIO_Port GPIOE
#define DSP_6_Pin GPIO_PIN_9
#define DSP_6_GPIO_Port GPIOE
#define DSP_7_Pin GPIO_PIN_10
#define DSP_7_GPIO_Port GPIOE
#define DSP_8_Pin GPIO_PIN_11
#define DSP_8_GPIO_Port GPIOE
#define DSP_9_Pin GPIO_PIN_12
#define DSP_9_GPIO_Port GPIOE
#define DSP_10_Pin GPIO_PIN_13
#define DSP_10_GPIO_Port GPIOE
#define DSP_11_Pin GPIO_PIN_14
#define DSP_11_GPIO_Port GPIOE
#define DSP_12_Pin GPIO_PIN_15
#define DSP_12_GPIO_Port GPIOE
#define DSP_13_Pin GPIO_PIN_8
#define DSP_13_GPIO_Port GPIOD
#define DSP_14_Pin GPIO_PIN_9
#define DSP_14_GPIO_Port GPIOD
#define DSP_15_Pin GPIO_PIN_10
#define DSP_15_GPIO_Port GPIOD
#define DSP_RS_Pin GPIO_PIN_13
#define DSP_RS_GPIO_Port GPIOD
#define DSP_0_Pin GPIO_PIN_14
#define DSP_0_GPIO_Port GPIOD
#define DSP_1_Pin GPIO_PIN_15
#define DSP_1_GPIO_Port GPIOD
#define DSP_2_Pin GPIO_PIN_0
#define DSP_2_GPIO_Port GPIOD
#define DSP_3_Pin GPIO_PIN_1
#define DSP_3_GPIO_Port GPIOD
#define DSP_RD_Pin GPIO_PIN_4
#define DSP_RD_GPIO_Port GPIOD
#define DSP_WR_Pin GPIO_PIN_5
#define DSP_WR_GPIO_Port GPIOD
#define DSP_CS_Pin GPIO_PIN_7
#define DSP_CS_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
