/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "tft9486.h"
#include "myimage.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern uint16_t BACK_COLOR,POINT_COLOR;
extern uint16_t tft_W;
extern uint16_t tft_H;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

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

  /* USER CODE BEGIN 2 */
  tft_init();
  tft_SetRotation(1);
  tft_clear(COLOR_BLACK);
  tft_draw_line(120,0,120,tft_W,COLOR_GREEN);
  tft_draw_circle(200,120,120,COLOR_VIOLET);
  tft_draw_rectangle(tft_H-20,0,tft_H,tft_W,COLOR_RED);
  tft_fill(tft_H-19,1,tft_H-1,tft_W-1,COLOR_YELLOW);
  BACK_COLOR=COLOR_RED;
  POINT_COLOR=COLOR_BLUE;
  tft_putchar(0,0,'A',TFT_STRING_MODE_NO_BACKGROUND);
  POINT_COLOR=COLOR_GREEN;
  tft_puts14x24(15,0,(int8_t*)"hi guys",TFT_STRING_MODE_NO_BACKGROUND);
  tft_puts14x24(40,0,(int8_t*)"brianx",TFT_STRING_MODE_NO_BACKGROUND);
  tft_puts14x24(70,0,(int8_t*)"Fleck",TFT_STRING_MODE_NO_BACKGROUND);
  tft_puts14x24(100,0,(int8_t*)"STM32F407",TFT_STRING_MODE_NO_BACKGROUND);

  tft_puts_image_pos(zzz, 240, 0, 240, 320);
  tft_puts_image_pos(qqq, 0,0,240,320);
  HAL_Delay(1000);
  POINT_COLOR=COLOR_YELLOW;
  tft_puts26x48(200,10,(int8_t*)"STM32F407",TFT_STRING_MODE_NO_BACKGROUND);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //  tft_puts_image(qqq);
    //  HAL_Delay(2000);
    //  tft_puts_image(aaa);
    //  HAL_Delay(2000);
    //  tft_puts_image(ddd);
    //  HAL_Delay(2000);
    //  tft_puts_image(zzz);
    //  HAL_Delay(2000);
    //  tft_puts_image(ttt);
    //  HAL_Delay(2000);
    //  tft_puts_image(nnn);
    //  HAL_Delay(2000);
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

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

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DSP_RST_GPIO_Port, DSP_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DSP_LED_GPIO_Port, DSP_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, DSP_4_Pin|DSP_5_Pin|DSP_6_Pin|DSP_7_Pin 
                          |DSP_8_Pin|DSP_9_Pin|DSP_10_Pin|DSP_11_Pin 
                          |DSP_12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DSP_13_Pin|DSP_14_Pin|DSP_15_Pin|DSP_RS_Pin 
                          |DSP_0_Pin|DSP_1_Pin|DSP_2_Pin|DSP_3_Pin 
                          |DSP_RD_Pin|DSP_WR_Pin|DSP_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DSP_RST_Pin */
  GPIO_InitStruct.Pin = DSP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DSP_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DSP_LED_Pin */
  GPIO_InitStruct.Pin = DSP_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DSP_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DSP_4_Pin DSP_5_Pin DSP_6_Pin DSP_7_Pin 
                           DSP_8_Pin DSP_9_Pin DSP_10_Pin DSP_11_Pin 
                           DSP_12_Pin */
  GPIO_InitStruct.Pin = DSP_4_Pin|DSP_5_Pin|DSP_6_Pin|DSP_7_Pin 
                          |DSP_8_Pin|DSP_9_Pin|DSP_10_Pin|DSP_11_Pin 
                          |DSP_12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : DSP_13_Pin DSP_14_Pin DSP_15_Pin DSP_0_Pin 
                           DSP_1_Pin DSP_2_Pin DSP_3_Pin */
  GPIO_InitStruct.Pin = DSP_13_Pin|DSP_14_Pin|DSP_15_Pin|DSP_0_Pin 
                          |DSP_1_Pin|DSP_2_Pin|DSP_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DSP_RS_Pin DSP_RD_Pin DSP_WR_Pin DSP_CS_Pin */
  GPIO_InitStruct.Pin = DSP_RS_Pin|DSP_RD_Pin|DSP_WR_Pin|DSP_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
