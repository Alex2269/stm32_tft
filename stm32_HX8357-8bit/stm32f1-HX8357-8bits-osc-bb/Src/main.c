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
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include "hx8357.h"
#include "utoa.h"
// #include "myimage.h"

#define M_PI 3.14159
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
extern uint16_t BACK_COLOR,POINT_COLOR;
extern uint16_t tft_W;
extern uint16_t tft_H;

uint32_t adcResult = 0;
uint32_t max_result = 0;
float real_result[480] = {0};

uint16_t adc_counter = 0;
uint16_t i = 0;

uint16_t buff_clean[480] = {0};
//uint16_t coord_x[480] = {0};
uint16_t buff_ray[480] = {0};

void write_text(uint16_t i);
void write_grid(void);

void fill_buff(uint16_t i)
{
  while(adc_counter < 48) // Пока количество измерений меньше N
  {
    adcResult += HAL_ADC_GetValue(&hadc1); // Сумируем полученные значения ацп
    adc_counter++; // Увеличиваем счетчик измерений
  }

  buff_ray[i] = tft_H-45-adcResult/adc_counter/18; // Вычисляем среднее значение
  real_result[i] = adcResult/adc_counter*0.82; // 3300/4096=0.80566 Вычисляем среднее значение

  // if(real_result[i]<500) real_result[i] = 0;
  // if(real_result[i]>500) real_result[i] -= 500;
  // real_result[i] *= .4; // значение в барах

  adcResult = 0; // Обнуляем значение
  adc_counter = 0; // Обнуляем счетчик измерений
  if(real_result[i]>max_result) max_result = real_result[i]; // save max value
}

void rotate_buff(uint16_t r)
{
    uint16_t tmp;
    for(i = 0; i < r; i++) // scroll buff_ray
    {
      tmp = buff_ray[tft_W-1];
      for(uint16_t i = tft_W-1; i; i--)
      {
        buff_ray[i] = buff_ray[i-1];
      }
      buff_ray[0] = tmp;
    }
}

void write_graph(uint16_t i)
{
  //write_text(i);
  if(i>0 && i < tft_W-0){ // clean old ray
    //tft_draw_point_big(i, buff_clean[i],COLOR_BLACK); //зарисовка предыдущего луча;
    //tft_draw_line_slim(i+1, buff_clean[i+1], i, buff_clean[i], COLOR_BLACK); //зарисовка предыдущего луча;
    tft_draw_dot_line(i+1, buff_clean[i+1], i, buff_clean[i], COLOR_BLACK); //зарисовка предыдущего луча;
  }
  if(i>2 && i < tft_W-2){ // draw new ray
    //tft_draw_point_big(i, buff_ray[i], COLOR_WHITE); //прорисовка текущего луча;
    //tft_draw_line_slim(i, buff_ray[i], i-1, buff_ray[i-1], COLOR_WHITE); //прорисовка текущего луча;
    tft_draw_dot_line(i, buff_ray[i], i-1, buff_ray[i-1], COLOR_WHITE); //прорисовка текущего луча;
  }
  buff_clean[i]=buff_ray[i];
}

void write_grid(void)
{
  for(uint16_t x = 32;x<tft_H-31;x+=8)for(uint16_t i = 16; i<tft_W-15; i+=64)tft_draw_point(i, x, COLOR_GRAYBLUE); //draw vertical dot line
  for(uint16_t x = 32;x<tft_H-31;x+=64)for(uint16_t i = 16; i<tft_W-15; i+=8)tft_draw_point(i, x, COLOR_WHITE); //draw horizontal dot line
}

void write_text(uint16_t i) // for text data
{
  char lbl_real[16] = "mv";
  BACK_COLOR=COLOR_BLUE;
  POINT_COLOR=COLOR_GREEN;
  tft_puts18x32(0,110,(int8_t*)lbl_real,TFT_STRING_MODE_BACKGROUND);

  char real_mv[16];
  utoa((int)real_result[i], real_mv, 10);
  if(real_result[i]<1000)tft_fill(1,62,32,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<100) tft_fill(1,44,32,80,BACK_COLOR); //x1 y1 x2 y2
  if(real_result[i]<10)  tft_fill(1,26,32,80,BACK_COLOR); //x1 y1 x2 y2
  tft_puts18x32(0,10,(int8_t*)real_mv,TFT_STRING_MODE_BACKGROUND);
}

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
  MX_TIM3_Init();
  MX_ADC1_Init();

  /* USER CODE BEGIN 2 */
  tft_init();
  tft_SetRotation(1);
  tft_clear(COLOR_BLACK);

  tft_draw_rectangle(31,0,0,tft_W,COLOR_RED);
  tft_fill(1,1,30,tft_W-1,COLOR_YELLOW); // x1,y1,x2,y2

  tft_draw_rectangle(tft_H-31,0,tft_H,tft_W,COLOR_RED);
  tft_fill(tft_H-30,1,tft_H-1,tft_W-1,COLOR_YELLOW);

  write_grid(); // draw grid
  write_text(i); // draw label & voltage measurement

  BACK_COLOR=COLOR_RED;
  POINT_COLOR=COLOR_BLUE;
  tft_putchar(tft_W/2,20,'A',TFT_STRING_MODE_NO_BACKGROUND);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_PeriodElapsedCallback(&htim3);
  HAL_TIM_Base_Start_IT(&htim3); //запустим таймер
  HAL_ADC_Start(&hadc1);//запустим аналогово-цифровое преобразование

  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    float t, a1, a2;

    t = 3300/2; // порог в миливольтах
    fill_buff(0); a1 = real_result[0];
    fill_buff(0); a2 = real_result[0];

    while (!((a1 < t) && (a2 >= t))) // т.е. было ниже порога, а стало больше или равно - был переход через порог
    {
      a1 = a2;
      fill_buff(0); a2 = real_result[0];
    }
    
    rotate_buff(5); // scrolling buffer

    for(uint16_t i = 0; i < tft_W; i++) // measurement
    {
      fill_buff(i);
    }

    for(uint16_t i = 0; i < tft_W; i++) // display redraw
    {
      write_graph(i);
    }

    write_grid(); // upgrade grid
    write_text(i); // upgrade label & voltage measurement

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
