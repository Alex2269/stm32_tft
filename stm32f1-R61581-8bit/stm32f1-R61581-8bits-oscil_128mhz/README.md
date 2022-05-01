# stm32f1-R61581-8bits-oscil_128mhz

``` cpp
 for change connected tft see Src/R61581.c
 in funtion tft_write_bus()

 GPIOA->ODR = byte;
 // GPIOA A0 A1 A2 A3 A4 A5 A6 A7
 // TFT   D0 D1 D2 D3 D4 D5 D6 D7

 GPIOA->ODR = (byte >> 2) | (byte << 6);
 // circular shift right of byte on 2 bits, 
 // to save from the cross direction of wires, on tft shield
 // GPIOA A0 A1 A2 A3 A4 A5 A6 A7  (byte >> 2) | (byte << 6)
 // TFT   D2 D3 D4 D5 D6 D7 D0 D1

 TFT    RD WR RS CS RST
 GPIOB  B5 B6 B7 B8 B9

 analog input GPIOB B0
```

1. Start by using Cube MX file in the root folder with the project with the extension ioc

2. Generate the project in the Cube MX and open it.

3. Compile the project: make and make flash.
