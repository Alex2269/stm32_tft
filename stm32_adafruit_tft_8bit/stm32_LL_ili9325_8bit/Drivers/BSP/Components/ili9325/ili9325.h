/**
  ******************************************************************************
  * @file    ili9325.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    02-December-2014
  * @brief   This file contains all the functions prototypes for the ili9325.c
  *          driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ILI9325_H
#define __ILI9325_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "../Common/lcd.h"

/** 
  * @brief  ILI9325 Size  
  */
#define  ILI9325_LCD_PIXEL_WIDTH    ((uint16_t)320)
#define  ILI9325_LCD_PIXEL_HEIGHT   ((uint16_t)480)

/**
  * @}
  */
  
/** @defgroup ILI9325_Exported_Functions
  * @{
  */ 
void     init_table8(const void *table, int16_t size);
void     init_table16(const void *table, int16_t size);
void     init_table_16_16(const void *table, int16_t size);
void     ili9325_Init(void);
uint16_t ili9325_ReadID(void);
void     ili9325_WriteReg(uint8_t LCD_Reg);
void     ili9325_WriteData(uint16_t RegValue);
uint32_t ili9325_ReadData(uint16_t RegValue, uint8_t ReadSize);
void     ili9325_DisplayOn(void);
void     ili9325_DisplayOff(void);
uint16_t ili9325_GetLcdPixelWidth(void);
uint16_t ili9325_GetLcdPixelHeight(void);

/* LCD driver structure */
extern LCD_DrvTypeDef   ili9325_drv;

/* LCD IO functions */
void     LCD_IO_Init(void);
void     tft_write_data(uint16_t RegValue);
void     tft_write_reg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void     LCD_Delay (uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif /* __ILI9325_H */

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
