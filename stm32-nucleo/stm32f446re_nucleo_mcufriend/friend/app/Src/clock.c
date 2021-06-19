#include "clock.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
  * @brief  Function called to read the current micro second
  * @param  None
  * @retval None
  */
uint32_t GetCurrentMicro(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  uint32_t u = SysTick->LOAD - SysTick->VAL;
  if(LL_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = SysTick->LOAD - SysTick->VAL;
  }
  return ( m * 1000 + (u * 1000) / SysTick->LOAD);
}

/**
  * @brief  Function called wto read the current millisecond
  * @param  None
  * @retval None
  */
uint32_t GetCurrentMilli(void)
{
  return HAL_GetTick();
}

void noOsSystickHandler(){

}

void osSystickHandler() __attribute__((weak, alias("noOsSystickHandler")));


/**
  * @brief  Function provides us delay (required by some arduino libraries).
  *         Can be called inside an interrupt.
  * @param  None
  * @retval None
  */
void delayInsideIT(uint32_t delay_us)
{
  uint32_t nb_loop;
#if defined (STM32F0xx) || defined (STM32L0xx)
  nb_loop = (((HAL_RCC_GetHCLKFreq() / 1000000)/5)*delay_us)+1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
  __asm__ volatile(
  "1: " "\n\t"
  " nop " "\n\t"
  " sub %0, %0, #1 " "\n\t"
  " bne 1b " "\n\t"
  : "=r" (nb_loop)
  : "0"(nb_loop)
  : "r3"
  );
#else
  nb_loop = (((HAL_RCC_GetHCLKFreq() / 1000000)/4)*delay_us)+1; /* uS (divide by 4 because each loop take about 4 cycles including nop +1 is here to avoid delay of 0 */
  __asm__ volatile(
  "1: " "\n\t"
  " nop " "\n\t"
  " subs.w %0, %0, #1 " "\n\t"
  " bne 1b " "\n\t"
  : "=r" (nb_loop)
  : "0"(nb_loop)
  : "r3"
  );
#endif
}

#ifdef __cplusplus
}//extern "C"
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
