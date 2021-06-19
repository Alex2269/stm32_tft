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
  * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "gfx.h"
#include "gdisp.h"
#include "ILI9488.h"
#include "math.h"
#define M_PI 3.14159
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

// A set of data points that will be displayed in the graph
static const point data[5] = 
{
  {-20, -20},
  { 35, 20 },
  { 70, 30 },
  {105, 30 },
  {140, 100}
};

// The graph object
static GGraphObject g;

// A graph styling
static GGraphStyle GraphStyle1 = 
{
  { GGRAPH_POINT_DOT, 0, Blue }, // Point
  { GGRAPH_LINE_NONE, 2, Gray }, // Line
  { GGRAPH_LINE_SOLID, 0, White }, // X axis
  { GGRAPH_LINE_SOLID, 0, White }, // Y axis
  { GGRAPH_LINE_DASH, 5, Gray, 50 }, // X grid
  { GGRAPH_LINE_DOT, 7, Yellow, 50 }, // Y grid
  GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS // Flags
};

// Another graph styling
static const GGraphStyle GraphStyle2 = 
{
  { GGRAPH_POINT_SQUARE, 5, Red }, // Point
  { GGRAPH_LINE_DOT, 2, Pink }, // Line
  { GGRAPH_LINE_SOLID, 0, White }, // X axis
  { GGRAPH_LINE_SOLID, 0, White }, // Y axis
  { GGRAPH_LINE_DASH, 5, Gray, 50 }, // X grid
  { GGRAPH_LINE_DOT, 7, Yellow, 50 }, // Y grid
  GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS // Flags
};

threadreturn_t heartbeat(void* param) 
{
  (void)param;
  while (TRUE)
  {
    gfxSleepMilliseconds(200);
  }
  return 0;
}

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
  MX_DMA_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOB,CTRL_WR_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,CTRL_RD_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,CTRL_RS_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,CTRL_CS_Pin,GPIO_PIN_RESET);

  MX_DMA_Init();
  MX_USART1_UART_Init();
  ///!
  GHandle gh;
  uint16_t i;

  // Initialize the uGFX library
  gfxInit();
  gdispSetOrientation(GDISP_ROTATE_270);   // flip by degrees

  // Create the graph window
  {
    GWindowInit wi;
    wi.show = TRUE;
    wi.x = wi.y = 0;
    wi.width = gdispGetWidth();
    wi.height = gdispGetHeight();
    gh = gwinGraphCreate(&g, &wi);
  }
    
  // Set the graph origin and style
  gwinGraphSetOrigin(gh, gwinGetWidth(gh)/2, gwinGetHeight(gh)/2);
  gwinGraphSetStyle(gh, &GraphStyle1);
  gwinGraphDrawAxis(gh);
  // Draw a sine wave
  for(i = 0; i < gwinGetWidth(gh); i++) 
  {
    gwinGraphDrawPoint(gh, i-gwinGetWidth(gh)/2, 80*sin(2*0.2*M_PI*i/180));
  }
  // Modify the style
  gwinGraphStartSet(gh);
  GraphStyle1.point.color = Green;
  gwinGraphSetStyle(gh, &GraphStyle1);
  // Draw a different sine wave
  for(i = 0; i < gwinGetWidth(gh)*5; i++) 
  {
    gwinGraphDrawPoint(gh, i/5-gwinGetWidth(gh)/2, 95*sin(5*0.2*M_PI*i/180));
  }
  // Change to a completely different style
  gwinGraphStartSet(gh);
  gwinGraphSetStyle(gh, &GraphStyle2);
  // Draw a set of points
  gwinGraphDrawPoints(gh, data, sizeof(data)/sizeof(data[0]));
  ///!

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    gfxSleepMilliseconds(100);
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

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
