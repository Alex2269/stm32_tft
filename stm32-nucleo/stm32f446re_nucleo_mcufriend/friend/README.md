# STM32CubeMX and adafruit_GFX
STM32CubeMX and adadfruit_GFX for TFT display.  
In this example I used ILI9341 (ST7789) driver with TFT display with a resolution of 320x240.  
The interfaces are 8-bit parallel.
The origin software is a arduino example from MCUFRIEND_kbv.
MCUFRIEND_kbv uses the adafruit_GFX for his own displays.
transfer the software no IDE with STM32CubeMX. Makefile
In this way I can see what happens.

attention:
if you regenerated the project, please:
remove Core/Src/main.c
remove Core/Src/tim.c

make
make flash
