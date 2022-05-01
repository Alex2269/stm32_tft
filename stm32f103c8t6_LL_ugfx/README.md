 
``` cpp

#  https://git.ugfx.io/uGFX/uGFX/src/v2.7
#  https://git.ugfx.io/uGFX/uGFX/src/v2.8
#  https://git.ugfx.io/uGFX/uGFX/src/v2.9
#  https://git.ugfx.io/uGFX/uGFX.git

stm32f103C8T6 minimal board + ILI9488 8 bit + ugfx

for remaping pins see: ILI9488.c 
func ILI9488WriteData8
func ILI9488ReadData

connect display:
display data port:
GPIOA first 8 pins for data
A0 A1 A2 A3 A4 A5 A6 A7 STM32 GPIOA
D0 D1 D2 D3 D4 D5 D6 D7 display pins

for remaping pins see: ILI9488.c func ILI9488WriteData8
shift for accurate distribution, pins tft shield
A0 A1 A2 A3 A4 A5 A6 A7 GPIO stm32
D2 D3 D4 D5 D6 D7 D0 D1 display pins

display control port:
B5 B6 B7 B8 B9  GPIOB
RD WR RS CS RST display port

run stm32cubeMx and generate  project:

git clone https://git.ugfx.io/uGFX/uGFX.git ugfx
unzip ILI9488_8b.zip -d ./
or
unzip R61581-8b.zip -d ./
make 
make flash

```
