# nucleo_f446re_uGUI

```cpp
 *  GPIO to data bus pin connections
 *  - PORT --- Pin --------- Data ---- ARDUINO_PIN
 *  GPIOA, GPIO_PIN_9   ->   BIT 0         D8
 *  GPIOC, GPIO_PIN_7   ->   BIT 1         D9
 *  GPIOA, GPIO_PIN_10  ->   BIT 2         D2
 *  GPIOB, GPIO_PIN_3   ->   BIT 3         D3
 *  GPIOB, GPIO_PIN_5   ->   BIT 4         D4
 *  GPIOB, GPIO_PIN_4   ->   BIT 5         D5
 *  GPIOB, GPIO_PIN_10  ->   BIT 6         D6
 *  GPIOA, GPIO_PIN_8   ->   BIT 7         D7

 *  GPIO to control bus pin connections
 *  - PORT --- Pin --------- Data ---- ARDUINO_PIN
 *  GPIOA, GPIO_PIN_0   ->   RD            A0
 *  GPIOA, GPIO_PIN_1   ->   WR            A1
 *  GPIOA, GPIO_PIN_4   ->   RD            A2
 *  GPIOB, GPIO_PIN_0   ->   CS            A3
 *  GPIOC, GPIO_PIN_1   ->   RST           A4
```
