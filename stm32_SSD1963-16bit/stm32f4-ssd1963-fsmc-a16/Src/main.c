/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "ssd1963_fsmc.h"
#include "xpt2046.h"

#include "fonts/Dmd8x7Clock.h"
#include "fonts/Dmd13x20Clock.h"
#include "fonts/FreeMono9pt7b.h"
#include "fonts/FreeMono12pt7b.h"
#include "fonts/FreeMono18pt7b.h"
#include "fonts/FreeMono24pt7b.h"
#include "fonts/FreeMonoBold9pt7b.h"
#include "fonts/FreeMonoBold12pt7b.h"
#include "fonts/FreeMonoBold18pt7b.h"
#include "fonts/FreeMonoBold24pt7b.h"
#include "fonts/FreeMonoBoldOblique9pt7b.h"
#include "fonts/FreeMonoBoldOblique12pt7b.h"
#include "fonts/FreeMonoBoldOblique18pt7b.h"
#include "fonts/FreeMonoBoldOblique24pt7b.h"
#include "fonts/FreeMonoOblique9pt7b.h"
#include "fonts/FreeMonoOblique12pt7b.h"
#include "fonts/FreeMonoOblique18pt7b.h"
#include "fonts/FreeMonoOblique24pt7b.h"
#include "fonts/FreeSans9pt7b.h"
#include "fonts/FreeSans12pt7b.h"
#include "fonts/FreeSans18pt7b.h"
#include "fonts/FreeSans24pt7b.h"
#include "fonts/FreeSansBold9pt7b.h"
#include "fonts/FreeSansBold12pt7b.h"
#include "fonts/FreeSansBold18pt7b.h"
#include "fonts/FreeSansBold24pt7b.h"
#include "fonts/FreeSansBoldOblique9pt7b.h"
#include "fonts/FreeSansBoldOblique12pt7b.h"
#include "fonts/FreeSansBoldOblique18pt7b.h"
#include "fonts/FreeSansBoldOblique24pt7b.h"
#include "fonts/FreeSansOblique9pt7b.h"
#include "fonts/FreeSansOblique12pt7b.h"
#include "fonts/FreeSansOblique18pt7b.h"
#include "fonts/FreeSansOblique24pt7b.h"
#include "fonts/FreeSerif9pt7b.h"
#include "fonts/FreeSerif12pt7b.h"
#include "fonts/FreeSerif18pt7b.h"
#include "fonts/FreeSerif24pt7b.h"
#include "fonts/FreeSerifBold9pt7b.h"
#include "fonts/FreeSerifBold12pt7b.h"
#include "fonts/FreeSerifBold18pt7b.h"
#include "fonts/FreeSerifBold24pt7b.h"
#include "fonts/FreeSerifBoldItalic9pt7b.h"
#include "fonts/FreeSerifBoldItalic12pt7b.h"
#include "fonts/FreeSerifBoldItalic18pt7b.h"
#include "fonts/FreeSerifBoldItalic24pt7b.h"
#include "fonts/FreeSerifItalic9pt7b.h"
#include "fonts/FreeSerifItalic12pt7b.h"
#include "fonts/FreeSerifItalic18pt7b.h"
#include "fonts/FreeSerifItalic24pt7b.h"
#include "fonts/FreeSevenSegNum.h"
#include "fonts/Org_01.h"
#include "fonts/Picopixel.h"
#include "fonts/Tiny3x3a2pt7b.h"
#include "fonts/TomThumb.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t touchIRQ = 0;
uint16_t touchX = 0, touchY = 0;
uint64_t millis = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_RNG_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_SPI3_Init();

  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start_IT(&htim1);
  LCD_Init();
  LCD_Rotate(1); // 1 or 2
  XPT2046_Init();
  uint8_t uartTransmit[] = "UART OK\r\n";
  HAL_UART_Transmit(&huart1, uartTransmit, sizeof(uartTransmit), 100);
  LCD_Rect_Fill(0, 0, 800, 480, BLUE);
  LCD_Rect_Fill(1, 1, 798, 478, BLACK);
  HAL_Delay(1000);
  LCD_Rect_Fill(0, 0, 160, 128, BLACK);
  for(uint8_t x = 8; x <= 160; x += 8)
  {
    LCD_Line(0, 0, x, 128, 1, GREEN);
  }
  for(uint8_t y = 8; y <= 128; y += 8) 
  {
    LCD_Line(0, 0, 160, y, 1, GREEN);
  }
  HAL_Delay(250);
  uint8_t h = 16;
  uint8_t w = 20;
  for(uint8_t i = 0; i < 8; i++)
  {
    LCD_Rect(80 - w / 2, 64 - h / 2, w, h, 2, YELLOW);
    h += 16;
    w += 20;
  }
  HAL_Delay(250);
  LCD_Rect_Fill(0, 0, 160, 128, BLUE);
  LCD_Rect_Fill(1, 1, 158, 126, BLACK);
  LCD_Font(5, 40, "This is\n just a Test\nST7735\n", Thumb, 1, YELLOW);
  LCD_Line(23, 20, 137, 20, 1, MAGENTA);
  LCD_Line(23, 21, 137, 21, 1, BLUE);
  LCD_Line(23, 21, 137, 21, 1, BLUE);
  LCD_Font(41, 10, "SSD1289 DRIVER", Org, 1, MAGENTA);
  LCD_Font(45, 35, "STM 32 HAL", SerifBold9, 1, RED);
  LCD_Circle(40, 90, 30, 0, 1, RED);
  LCD_Circle(45, 90, 20, 1, 1, BLUE);
  LCD_Triangle(5, 5, 5, 20, 25, 25, 2, BLUE);
  LCD_Rect(60, 45, 30, 20, 2, GREEN);
  LCD_Rect_Round(80, 70, 60, 25, 10, 3, WHITE);
  LCD_Rect_Round_Fill(80, 100, 60, 25, 10, WHITE);
  LCD_Ellipse(60, 100, 30, 20, 0, 2, YELLOW);
  LCD_Ellipse(125, 60, 25, 15, 1, 1, YELLOW);
  LCD_Font(0, 200, "1234567890", SevenSegNum, 1, RED);
  LCD_Font(10, 220, "1234567890 TEST FONT", Clock8x7, 1, RED);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (touchIRQ) 
    {
      touchX = getX(); 
      touchY = getY();
      //LCD_Pixel(touchX, touchY, WHITE);
      LCD_Rect_Fill(touchX, touchY, 2, 2, WHITE);  
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
      touchX = 0;
      touchY = 0;
      touchIRQ = 0;
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    }
    if (millis / 1000 % 2 == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }

  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
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
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == GPIO_PIN_3) touchIRQ = 1;
 else { __NOP(); }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
