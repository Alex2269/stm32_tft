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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "rng.h"
#include "usb_host.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "stdint.h"
#include "ili9341.h"

#define M_PI 3.14159
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
extern uint16_t BACK_COLOR,POINT_COLOR;
extern uint16_t tft_W;
extern uint16_t tft_H;

extern ApplicationTypeDef Appli_state;
FATFS USBDISKFatFs;
FIL MyFile;
extern USBH_HandleTypeDef hUsbHostFS;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//------------------------------------
__STATIC_INLINE void DelayMicro(uint32_t __IO micros)
{
  micros *=(SystemCoreClock/1000000)/5;
  while(micros--);
}

//------------------------------------

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //char str[100];
	uint16_t i,j;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_RNG_Init();
  MX_FATFS_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(STM_LED1_GPIO_Port, STM_LED1_Pin, GPIO_PIN_RESET); // LED1
  HAL_GPIO_WritePin(STM_LED2_GPIO_Port, STM_LED2_Pin, GPIO_PIN_RESET); // LED2
  HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET); // LIGHT

  tft_ini();
  tft_clear(BLACK);
  HAL_Delay(1000);
  tft_clear(RED);
  HAL_Delay(1000);
  tft_clear(BLACK);

  tft_SetRotation(1);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // create grid
  for(uint16_t i = 5; i<tft_H-5; i+=30)
  {
   tft_DrawLine(COLOR_GRAY, 5, i, tft_W-5, i);
  }

  for(uint16_t i = 5; i<tft_W-5; i+=30)
  {
   tft_DrawLine(COLOR_GRAY, i, 5, i, tft_H-5);
  }

  BACK_COLOR=BLACK;
  POINT_COLOR=BLUE;
  tft_putchar26x48(90,20,'A',TFT_STRING_MODE_BACKGROUND);
  BACK_COLOR=BLACK;
  POINT_COLOR=YELLOW;
  tft_putchar26x48(160,20,'B',TFT_STRING_MODE_BACKGROUND);
  tft_puts26x48(200,10,(int8_t*)"STM32F407",TFT_STRING_MODE_NO_BACKGROUND);
  tft_puts14x24(150,10,(int8_t*)"STM32F407",TFT_STRING_MODE_NO_BACKGROUND);
  tft_puts18x32(100,10,(int8_t*)"STM32F407",TFT_STRING_MODE_NO_BACKGROUND);
  HAL_Delay(3000);

//     tft_DrawLine(color, x1, y1, x2, y2);

  while (1)
  {
    for(float i = 0; i < tft_W; i+=0.1) // count point
    {
      tft_DrawPixel(i, 80*sin(30*0.2*M_PI*i/180)+120,  COLOR_GREEN);
    }
    HAL_Delay(3000);

    for(uint16_t i = 0; i < tft_W; i++)
    {
      tft_DrawPixel(i, 95*sin(5*0.2*M_PI*i/180)+120,  COLOR_YELLOW);
    }
    HAL_Delay(3000);

    for(i=0;i<15000;i++)
    {
      tft_DrawPixel(HAL_RNG_GetRandomNumber(&hrng)%tft_W, HAL_RNG_GetRandomNumber(&hrng)%tft_H, tft_RandColor());
      DelayMicro(100);
    }

    tft_FillScreen(BLACK);
    HAL_Delay(250);

    for(j=0;j<20;j++)
    {
      for(i=0;i<tft_W;i++)
      {
        tft_DrawLine(tft_RandColor(),i,0,i,tft_H-1);
      }
    }

    tft_FillScreen(BLACK);
    HAL_Delay(250);

    for(i=0;i<1000;i++)
    {
      tft_DrawLine(tft_RandColor(),
      HAL_RNG_GetRandomNumber(&hrng)%tft_W,
      HAL_RNG_GetRandomNumber(&hrng)%tft_H,
      HAL_RNG_GetRandomNumber(&hrng)%tft_W,
      HAL_RNG_GetRandomNumber(&hrng)%tft_H);
      HAL_Delay(10);
    }

    tft_FillScreen(BLACK);
    HAL_Delay(250);

    for(j=0;j<20;j++)
    {
      for(i=0;i<tft_W/2;i++)
      {
        tft_DrawRect(tft_RandColor(), i, i, tft_W-i-1, tft_H-i-1);
      }
    }

    tft_FillScreen(BLACK);
    HAL_Delay(250);

    for(i=0;i<2000;i++)
    {
      tft_DrawCircle(HAL_RNG_GetRandomNumber(&hrng)%(tft_W-40)+20,
      HAL_RNG_GetRandomNumber(&hrng)%(tft_H-40)+20,
      20,tft_RandColor());
      HAL_Delay(3);
    }

    tft_FillScreen(BLACK);
    HAL_Delay(250);

    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
