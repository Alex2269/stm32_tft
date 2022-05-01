#ifndef _ILI9488_H
#define _ILI9488_H
#include "stm32f1xx_hal.h"

//GPIOA first 8 pins for data
//this pin is called rs on my dev board
// A0 A1 A2 A3 A4 A5 A6 A7 STM32 GPIOA
// D0 D1 D2 D3 D4 D5 D6 D7 TFT   DATA PORT

#define LCD_BUS GPIOA
#define WRITE_LCD(data) LCD_BUS->BSRR = (LCD_BUS->BSRR & 0xff00ff00) | (uint32_t)(((0x00ff & ~data) << 16u)| (0x00ff & data));
#define READ_LCD (LCD_BUS->IDR & 0b0000000011111111)

void BUS_output(void);
void BUS_input(void);
void ILI9488DisplayOn(void);
void ILI9488DisplayOff(void);
void ILI9844NormalDisplayModeOn(void);
unsigned long ILI9488_ReadID(void);
unsigned long ILI9488ReadDisplayStatus(void);
void ILI9844AllPixelOn(void);
void ILI9844AllPixelOff(void);

void ILI9488_Initial_Code(void);
void ILI9488MemoryRead(unsigned short * pData, unsigned short size);
void ILI9488WriteReg(uint16_t data) ;
void ILI9488WriteData(uint16_t data) ;
void ILI9488Write8bit(uint8_t data);
uint16_t ILI9488ReadData(void) ;

#endif
