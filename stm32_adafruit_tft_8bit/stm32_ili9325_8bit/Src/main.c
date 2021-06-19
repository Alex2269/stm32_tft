/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

#include "clock.h"
#include "gauge.h"
#include "graphics_test.h"
#include "stm32_adafruit_lcd.h"
#include "ili9325_io.h"
#include "ili9325.h"
#include "System_Clock_128.h"

#define M_PI 3.14159

//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-function"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  System_Clock_128();

  /* USER CODE BEGIN SysInit */
  // System_Clock_128();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  BSP_LCD_DisplayOff();
  BSP_LCD_Clear(LCD_COLOR_RED);
  BSP_LCD_DisplayOn();
  HAL_Delay(100);
  BSP_LCD_SetRotation(2);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  while (1)
  {
    //lcd_test();

    //BSP_LCD_Clear(LCD_COLOR_BLACK);
    // clock();
    for(uint16_t i=270;i<=360;i++)
    { //(coordinate_x, coordinate_y, rotateangle ,color, value)
      //BSP_LCD_Clear(LCD_COLOR_BLACK);
      gauge(24,36, 100,LCD_COLOR_BLUE, i);
      // gauge(10,60, 100,LCD_COLOR_YELLOW, i);
      // gauge(165,10, 100,LCD_COLOR_RED, i);
      // gauge(165,160, 100,LCD_COLOR_CYAN, i);
      // gauge(10,310, 100,LCD_COLOR_WHITE, i);
      // gauge(165,310, 100,LCD_COLOR_WHITE, i);
      //HAL_Delay(5);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
