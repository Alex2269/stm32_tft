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
  * COPYRIGHT(c) 2019 STMicroelectronics
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
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "SetSysClockTo128.h"
#include "pins_ext.h"
#include "bit_band.h"
#include "soft_spi.h"
#include "amplifier.h"
// #include "enc.h"
#include <math.h>
#include "osc.h"
#include "tft_base.h"
#include "utoa.h"
#include "icons.h"
#include "tft_adafruit.h"
#include "tft_gfx.h"
#include "tft_gfx2.h"
// #include "myimage.h"

#include "FreeSans9pt7b.h"

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

#define SYSCLK_FREQ_128MHz

#define M_PI 3.14159
#define WIDTH 400 // set_width tft
#define HEIGHT 240 // set_height tft

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern bitband_t mosi;
extern bitband_t clock;
extern bitband_t amplifier_cs;

extern TIM_HandleTypeDef htim3;

extern uint16_t tft_W;
extern uint16_t tft_H;
extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

extern uint8_t penguin[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void test_screen(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

uint16_t array_sin[WIDTH];
uint16_t array_cos[WIDTH];

void test_screen(void)
{
  for(uint16_t i = 0; i < tft_W; i++) // count point
  {
    array_cos[i]=(float)48*cos(30*0.05*M_PI*i/180)+100;
    array_sin[i]=(float)48*sin(30*0.05*M_PI*i/180)+140;
  }

  for(uint16_t i = 0x00e0; i<0x00f0; i += 0x1)
  {
    uint16_t color = i;
    BACK_COLOR=color;
    POINT_COLOR=~color & 0xffff;
    fastRect(0, 0, tft_W, 240, i);
    // tft_fill_grid(24/*y1*/, 0/*x1*/, 240-24/*y2*/, tft_W/*x2*/, i);

    setAddrWindow(tft_W/2 - 20 + 0, tft_H/2 - 20 + 0, tft_W/2 - 20 + 39, tft_H/2 - 20 + 39);
    // pushColors1(penguin, 1600, 1);
    // pushColors3(penguin, 1600, 1, false);
    write_grid();
  }

  for(uint16_t i = 50; i < tft_W-50; i++)
  {
    uint16_t color = i;
    BACK_COLOR=color;
    POINT_COLOR=~color & 0xffff;
    // tft_draw_point(i,array_cos[i], ~color & 0xffff);
    // tft_draw_point(i,array_sin[i], ~color & 0xffff);
    fillCircle(i, array_cos[i], 2, COLOR_BLUE);
    fillCircle(i, array_sin[i], 5, COLOR_GREEN);
  }

  setAddrWindow(tft_W/2 - 20 + 0, tft_H/2 - 20 + 0, tft_W/2 - 20 + 39, tft_H/2 - 20 + 39);
  pushColors3(penguin, 1600, 1, false);
  HAL_Delay(2000);
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  mosi = BITBAND_PERIPH(&GPIOB->ODR, 12);
  clock = BITBAND_PERIPH(&GPIOB->ODR, 14);
  amplifier_cs = BITBAND_PERIPH(&GPIOA->ODR, 8);

  pin_mode(GPIOB, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP); // lcd mosi
  pin_mode(GPIOB, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP); // lcd clk
  pin_mode(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP); // lcd cs

  soft_spi_set_bit_order(SPI_MSB_FIRST);
  soft_spi_set_data_mode(SPI_MODE0);
  soft_spi_set_clock_divider(SPI_CLOCK_DIV2);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_RCC_DeInit();
  SystemClock_Config94(); // see makefile: if optimisation enabled flag OPT = -O2 or OPT = -O3, use 94 MGz
  // SystemClock_Config128(); // see makefile: if optimisation disable flag OPT = -Og, use 128 MGz

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_PeriodElapsedCallback(&htim3);
  HAL_TIM_Base_Start_IT(&htim3); //запустим таймер
  
  HAL_TIM_Base_Init(&htim4);
  HAL_TIM_PeriodElapsedCallback(&htim4);
  HAL_TIM_Base_Start_IT(&htim4); //запустим таймер

  HAL_ADC_Start(&hadc1);//запустим аналогово-цифровое преобразование
  HAL_ADC_Start(&hadc2);//запустим аналогово-цифровое преобразование

  set_width(WIDTH);
  set_height(HEIGHT);

  init_tft();
  setRotation(1);
  test_screen();

  setFont(&FreeSans9pt7b);

  setTextSize(2);
  setCursor(100, 100);
  setTextColor(COLOR_GREEN);
  write('+');

  drawChar(40, 60, 'R', COLOR_RED, 0, 2);
  drawChar(40, 120, 'G', COLOR_GREEN, 0, 2);
  drawChar(40, 180, 'B', COLOR_BLUE, 0, 2);

  HAL_Delay(1000);

  setAddrWindow(tft_W/2 - 20 + 0, tft_H/2 - 20 + 0, tft_W/2 - 20 + 39, tft_H/2 - 20 + 39);
  // pushColors1(penguin, 1600, 1);
  pushColors3(penguin, 1600, 1, false);
  
  HAL_Delay(1000);

  oscil_init();

  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    oscil_run();

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
