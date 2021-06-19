#include "main.h"
#include "touch_hall.h"

#define GPIO_Read(port,pin) HAL_GPIO_ReadPin(port,pin)
#define GPIO_Write(port,pin,state) HAL_GPIO_WritePin(port,pin,state)

void Touch_ini(void)
{
  GPIO_Write(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin,1);
  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,1);
  GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin,1);
  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,1);
}

void spi_ts_write(uint8_t TxData)
{
  uint8_t count=0;
  for(count=0;count<8;count++)
  {
    if((TxData&0x80)==0x80)
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin,1);
    }
    else
    {
      GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin,0);
    }

    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,0);
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,1);

    TxData=TxData<<1;
  }
}

uint16_t ADS_Read_AD(unsigned char CMD)
{
  uint8_t count=0;
  uint16_t num=0,a=450;

  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,0);
  GPIO_Write(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin,0);
  GPIO_Write(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin,0);

  spi_ts_write(CMD);

  while(a--);
  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,0);
  a=72;

  while(a--);
  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,1);
  GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,0);

  for(count=0;count<16;count++)
  {
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,0);
    num<<=1;
    GPIO_Write(TOUCH_CLK_GPIO_Port, TOUCH_CLK_Pin,1);
    if(GPIO_Read(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin)==1) num++;
  }

  num>>=4;

  GPIO_Write(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin,1);
  return num;
}

#define READ_TIMES 15
#define LOST_VAL 5

uint16_t ADS_Read_XY(uint8_t xy)
{
  uint16_t i, j;
  uint16_t buf[READ_TIMES];
  uint16_t sum=0;
  uint16_t temp;

  for(i=0;i<READ_TIMES;i++)
  {
    buf[i]=ADS_Read_AD(xy);
  }

  for(i=0;i<READ_TIMES-1; i++)
  {
    for(j=i+1;j<READ_TIMES;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }

  sum=0;

  for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];

  temp=sum/(READ_TIMES-2*LOST_VAL);

  return temp;
}

int get_touch(TS_Point *p)
{
  uint16_t xtemp,ytemp;

  xtemp=ADS_Read_XY(CMD_RDX);
  ytemp=ADS_Read_XY(CMD_RDY);

  if(xtemp<100||ytemp<100)return 0;
  p->x = xtemp;
  p->y = ytemp;

  return 1;
}

