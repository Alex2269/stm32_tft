#include "ILI9488.h"
#include "stm32f1xx_hal.h"
#include "main.h"

/* include for HAL_Delay */

void BUS_output(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pins : PB0 PB10 PB11 PB12
  PB8 PB9 */
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BUS, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                            |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                       |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_BUS, &GPIO_InitStruct);
}

void BUS_input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Configure GPIO pin Output Level */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                       |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_BUS, &GPIO_InitStruct);

  HAL_GPIO_WritePin(LCD_BUS, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                            |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
}

void ILI9488WriteData8(uint8_t byte)
{
  //:! shift for accurate distribution, pins tft shield
  //:! A0 A1 A2 A3 A4 A5 A6 A7 GPIO stm32
  //:! D2 D3 D4 D5 D6 D7 D0 D1 display pins

  uint8_t shift;
  shift=(byte >> 2) | (byte << 6);
  // GPIOA->ODR = byte;
  GPIOA->ODR = shift;
  // WRITE_LCD(byte);
  // WRITE_LCD(shift);

  HAL_GPIO_WritePin(GPIOB, CTRL_WR_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, CTRL_WR_Pin,GPIO_PIN_SET);
}

void ILI9488WriteReg(uint16_t data)
{
  HAL_GPIO_WritePin(GPIOB, CTRL_RS_Pin,GPIO_PIN_RESET);
  ILI9488WriteData8(data&0xff);
  HAL_GPIO_WritePin(GPIOB, CTRL_RS_Pin,GPIO_PIN_SET);
}

void ILI9488WriteData(uint16_t data)
{
  ILI9488WriteData8(data>>8);
  ILI9488WriteData8(data&0xff);
}

uint16_t ILI9488ReadData(void)
{
  uint16_t data;
  BUS_input();
  HAL_GPIO_WritePin(GPIOB, CTRL_RS_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, CTRL_WR_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_RESET);
  data=READ_LCD;
  HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_SET);
  BUS_output();

  //return data;
  uint8_t shift;
  shift=(data >> 2) | (data << 6);
  return shift;
}

void ILI9488DisplayOn()
{
  ILI9488WriteReg(0x29);
}

void ILI9488DisplayOff()
{
  ILI9488WriteReg(0x28);
}

void ILI9488MemoryWrite(unsigned short * pData, unsigned long size)
{
  unsigned long i;
  ILI9488WriteReg(0x2C);
  for(i=0;i<size;i++)
  {
    ILI9488WriteData(*pData++);
  }
}

void ILI9488RamAddress(void)
{
  // set column address
  ILI9488WriteReg(0x2a);
  ILI9488WriteData(0x00);
  ILI9488WriteData(0x00);
  ILI9488WriteData(0x01);
  ILI9488WriteData(0x3f);
  // set page address
  ILI9488WriteReg(0x2b);
  ILI9488WriteData(0x00);
  ILI9488WriteData(0x00);
  ILI9488WriteData(0x01);
  ILI9488WriteData(0xdf);
  // send command memory write
  ILI9488WriteReg(0x2c);
}

void ILI9844NormalDisplayModeOn()
{
  ILI9488WriteReg(0x13);
}

void ILI9844AllPixelOn()
{
  ILI9488WriteReg(0x23);
}

void ILI9844AllPixelOff()
{
  ILI9488WriteReg(0x22);
}

void ILI9844ColumnAddressSet(unsigned short SC, unsigned short EC)
{
  ILI9488WriteReg(0x2A);
  ILI9488WriteData( (SC & 0xFF00) >> 8);
  ILI9488WriteData(SC & 0xFF);
  ILI9488WriteData( (EC & 0xFF00) >> 8);
  ILI9488WriteData(EC & 0xFF);
}

void Ili9844PageAddressSet(unsigned short SP, unsigned short EP)
{
  ILI9488WriteReg(0x2B);
  ILI9488WriteData( (SP & 0xFF00) >> 8);
  ILI9488WriteData(SP & 0xFF);
  ILI9488WriteData( (EP & 0xFF00) >> 8);
  ILI9488WriteData(EP & 0xFF);
}

void ILI9488MemoryRead(unsigned short * pData, unsigned short size)
{
  unsigned short i;
  ILI9488WriteReg(0x2E);
  BUS_input();
  for(i=0;i<size;i++)
  {
    HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_RESET);
    *pData++ = READ_LCD;
    HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_SET);
  }
  BUS_output();
}

unsigned long ILI9488ReadDisplayStatus()
{
  volatile unsigned long tmp;
  ILI9488WriteReg(0x09);
  BUS_input();
  tmp=ILI9488ReadData()<<24;
  tmp=ILI9488ReadData()<<16;
  tmp=ILI9488ReadData()<<8;
  tmp=ILI9488ReadData();
  BUS_output();
  return tmp;
}

unsigned long ILI9488_ReadID(void)
{
  unsigned char buf[4];
  ILI9488WriteReg(0x04);
  BUS_input();
  buf[0] = ILI9488ReadData();
  buf[1] = ILI9488ReadData();
  buf[2] = ILI9488ReadData();
  buf[3] = ILI9488ReadData();
  BUS_output();
  return (buf[1]<<16)+(buf[2]<<8)+(buf[3]);
}

unsigned short ILI9844ReadNumberOfTheErrorsOnDsi()
{
  volatile unsigned short tmp;
  ILI9488WriteReg(0x05);
  BUS_input();
  HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_RESET);
  tmp = READ_LCD;
  HAL_GPIO_WritePin(GPIOB, CTRL_RD_Pin,GPIO_PIN_SET);
  BUS_output();
  return tmp;
}
