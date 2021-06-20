# STM32CubeMX and µGUI with TouchScreen

STM32CubeMX and µGUI for all display.  
In this example I used a R61581 driver with TFT display with a resolution of 320x240
and a nucleo board 64 with a STM32F103C8T6.
The interface is 8-bit parallel.
In this repository I used the graphic library with a touch screen

[The origin software is here](http://embeddedlightning.com/ugui/)

The screen looks like this picture

![Start](doc/Start.JPG)

harebit

```
 connect:
 tft io  stm32 io

 LCD_RST GPIOB_3
 LCD_CS  GPIOB_4
 LCD_DC  GPIOB_5
 LCD_WR  GPIOB_6
 LCD_RD  GPIOB_7
 
 LCD_D0  GPIOB_8
 LCD_D1  GPIOB_9
 LCD_D2  GPIOB_10
 LCD_D3  GPIOB_11
 LCD_D4  GPIOB_12
 LCD_D5  GPIOB_13
 LCD_D6  GPIOB_14
 LCD_D7  GPIOB_15
 
```
