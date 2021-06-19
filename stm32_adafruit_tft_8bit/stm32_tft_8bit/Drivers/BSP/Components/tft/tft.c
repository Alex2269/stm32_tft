/**
  ******************************************************************************
  * @file    tft.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    02-December-2014
  * @brief   This file includes the LCD driver for tft LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "tft_init.h"
#include "tft_io.h"
#include "tft.h"
#include "stm32f1xx_hal.h"

#include "gpio.h"

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup tft
  * @brief This file provides a set of functions needed to drive the 
  *        tft LCD.
  * @{
  */

/** @defgroup tft_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup tft_Private_Defines
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup tft_Private_Macros
  * @{
  */
/**
  * @}
  */  

/** @defgroup tft_Private_Variables
  * @{
  */ 

LCD_DrvTypeDef   tft_drv = 
{
  tft_Init,
  tft_ReadID,
  tft_DisplayOn,
  tft_DisplayOff,
  0,
  0,
  0,
  0,
  0,
  0,
  tft_GetLcdPixelWidth,
  tft_GetLcdPixelHeight,
  0,
  0,    
};

/**
  * @}
  */ 
  
/** @defgroup tft_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup tft_Private_Functions
  * @{
  */   
void init_table8(const void *table, int16_t size)
{
  uint8_t *p = (uint8_t *) table;
  while (size > 0)
  {
    uint8_t cmd = *p++;
    uint8_t len = *p++;
    if (cmd == TFTLCD_DELAY8)
    {
      HAL_Delay(len);
      len = 0;
    }
    else
    {
      tft_write_reg(cmd);
      for (uint8_t i = 0; i < len; i++)
      if(len)
      {
        tft_write_data(*p++);
      }
    }
    size -= len + 2 * sizeof(int8_t);
  }
}

void init_table16(const void *table, int16_t size)
{
  uint16_t *p = (uint16_t *) table;
  while (size > 0)
  {
    uint16_t dat;
    uint16_t cmd = *p++;
    uint16_t len = *p++;
    if (cmd == TFTLCD_DELAY)
    {
      HAL_Delay(len);
      len = 0;
    }
    else
    {
      tft_write_reg(cmd);
      for (uint16_t i = 0; i < len; i++)
      if(len)
      {
        dat = *p++;
        tft_write_data(dat>>8);
        tft_write_data(dat&0x00ff);
      }
    }
    size -= len + 2 * sizeof(int16_t);
  }
}

void init_table_16_16(const void *table, int16_t size)
{
  uint16_t *p = (uint16_t *) table;
  while (size > 0)
  {
    uint16_t dat;
    uint16_t cmd = *p++;
    uint16_t len = *p++;
    if (cmd == TFTLCD_DELAY)
    {
      HAL_Delay(len);
      len = 0;
    }
    else
    {
      tft_write_reg16(cmd);
      for (uint16_t i = 0; i < len; i++)
      if(len)
      {
        dat = *p++;
        tft_write_data16(dat);
      }
    }
    size -= len + 2 * sizeof(int16_t);
  }
}

/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
void tft_Init(void)
{
  /* Initialize tft low level bus layer ----------------------------------*/
  LCD_IO_Init();
  pin_high(LED_GPIO_Port,LED_Pin);
  pin_high(LCD_CS_GPIO_Port,LCD_CS_Pin);
  pin_high(LCD_RD_GPIO_Port,LCD_RD_Pin);
  pin_high(LCD_WR_GPIO_Port,LCD_WR_Pin);
  pin_low(LCD_RST_GPIO_Port,LCD_RST_Pin);
  HAL_Delay(20);
  pin_high(LCD_RST_GPIO_Port,LCD_RST_Pin);
  HAL_Delay(20);

  pin_low(LCD_CS_GPIO_Port,LCD_CS_Pin);

  //cmd number-data data's
  tft_WriteReg(0x01);//Software Reset

  init_table8(&init_array_R61581_8, sizeof(init_array_R61581_8)); // #include "init_R61581.h"
  // init_table8(&init_array_hx8357_8, sizeof(init_array_hx8357_8)); // #include "init_R61581.h"
  // init_table16(&init_array_R61581_16, sizeof(init_array_R61581_16)); // #include "init_R61581.h"
  // init_table16(&init_array_ili9325_16, sizeof(init_array_ili9325_16)); // #include "init_R61581.h"
  /* Configure LCD */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval LCD Register Value.
  */
uint16_t tft_ReadID(void)
{
  LCD_IO_Init();
  return ((uint16_t)tft_ReadData(LCD_READ_ID4, LCD_READ_ID4_SIZE));
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void tft_DisplayOn(void)
{
  /* Display On */
  tft_WriteReg(LCD_DISPLAY_ON);
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void tft_DisplayOff(void)
{
  /* Display Off */
  tft_WriteReg(LCD_DISPLAY_OFF);
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
}

/**
  * @brief  Writes  to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void tft_WriteReg(uint8_t LCD_Reg)
{
  LCD_IO_WriteReg(LCD_Reg);
}

/**
  * @brief  Writes data to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void tft_WriteData(uint16_t RegValue)
{
  LCD_IO_WriteData(RegValue);
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  RegValue: Address of the register to read
  * @param  ReadSize: Number of bytes to read
  * @retval LCD Register Value.
  */
uint32_t tft_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  /* Read a max of 4 bytes */
  return (LCD_IO_ReadData(RegValue, ReadSize));
}

/**
  * @brief  Get LCD PIXEL WIDTH.
  * @param  None
  * @retval LCD PIXEL WIDTH.
  */
uint16_t tft_GetLcdPixelWidth(void)
{
  /* Return LCD PIXEL WIDTH */
  return tft_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get LCD PIXEL HEIGHT.
  * @param  None
  * @retval LCD PIXEL HEIGHT.
  */
uint16_t tft_GetLcdPixelHeight(void)
{
  /* Return LCD PIXEL HEIGHT */
  return tft_LCD_PIXEL_HEIGHT;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
