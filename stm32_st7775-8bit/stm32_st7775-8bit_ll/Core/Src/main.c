/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "st7775.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define delay_ms(ms) LL_mDelay(ms)
extern uint16_t BACK_COLOR,POINT_COLOR;
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

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  InitDisplay();

  DrawArea(0,0, 220,176, COLOR_BLACK); // ClearDisplay();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    DrawArea(0,0, 50,50, COLOR_CYAN);
    DrawHLine(10, 20, 50, COLOR_RED);
    fillCircle(130,35, 30, COLOR_YELLOW);
    fillCircle(160,75, 30, COLOR_BLUE);
    fillCircle(180,105, 10, COLOR_PINK);
    DrawShape8x8(100,50, 9, 5, 1, 0, 0);

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollLeft();
      delay_ms(10);
    }

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollRight();
      delay_ms(10);
    }

    DrawArea(0,0, 220,176, COLOR_DARKGREY); // ClearDisplay();
    DrawArea(0,0, 220,176, COLOR_DARKGREY);
    DrawArea(55,55, 40,40, COLOR_WHITE);
    DrawArea(100,100, 30,30, COLOR_ORANGE);
    DrawArea(135,135, 20,20, COLOR_GREENYELLOW);

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollLeft();
      delay_ms(10);
    }

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollRight();
      delay_ms(10);
    }

    DrawArea(0,0, 220,176, COLOR_BLACK); // ClearDisplay();
    BACK_COLOR=COLOR_BLACK;
    POINT_COLOR=COLOR_BLUE;
    tft_putchar26x48(10,10,'A',TFT_STRING_MODE_BACKGROUND);
    POINT_COLOR=COLOR_YELLOW;
    tft_putchar26x48(60,10,'B',TFT_STRING_MODE_BACKGROUND);
    POINT_COLOR=COLOR_CYAN;
    tft_puts26x48(60,10,(int8_t*)"str1",TFT_STRING_MODE_NO_BACKGROUND);
    POINT_COLOR=COLOR_GREENYELLOW;
    tft_puts14x24(110,10,(int8_t*)"str2",TFT_STRING_MODE_NO_BACKGROUND);
    POINT_COLOR=COLOR_ORANGE;
    tft_puts18x32(140,10,(int8_t*)"string3",TFT_STRING_MODE_NO_BACKGROUND);

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollLeft();
      delay_ms(10);
    }

    for(uint8_t i = 0;i<255;i++)
    {
      ScrollRight();
      delay_ms(10);
    }

    DrawArea(0,0, 220,176, COLOR_BLACK); // ClearDisplay();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
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
