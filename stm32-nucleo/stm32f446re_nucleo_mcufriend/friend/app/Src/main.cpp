#include "Clock_Config.h"
#include <main.hpp>
#include <main.h>
#include "gpio.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  setup();
  while (1)
  {
    loop();
  }
}

