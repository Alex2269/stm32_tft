#include "osc.h"
#include "main.h"
#include "enc.h"

#define SPEED_MIN 1
#define SPEED_MAX 131072

#define SCROLL_Y_MIN (-216)
#define SCROLL_Y_MAX 216

#define MARKRECT 2
#define ybar (tft_H-18)
#define offset 4
#define xbar 40

#define debug 1
#define debug_menu 1
#define debug_position 1

static uint16_t menu_pos = 1;
static uint16_t pressed = 1; // active menu

void delay_enc(uint32_t tics)
{
  uint32_t count=0;
  for(count=0;count<tics;count++)
  {
    __asm volatile ("nop");
  }
}

void buttons(void)
{
  pressed++;
  if(pressed >= 2 ) pressed = 0;
  #if debug
  if(pressed)
  {
    write_marker_rect(8, tft_H-14, 7, COLOR_GREEN);
    write_str(120, 0, "m"); // menu
  }
  if(!pressed)
  {
    write_marker_rect(8, tft_H-14, 7, COLOR_DARKBLUE);
    write_str(120, 0, "v"); // value
  }
  write_enc_val(360, 4, pressed);
  #endif
  delay_enc(100);
}

void encoder_rising(void)
{
  oscil_stop = 1;
  if(pressed)
  {
    menu_pos --;
    if( menu_pos < 2) menu_pos = 1;
    #if debug_position
      write_enc_val(380, tft_H-18, menu_pos);
      write_val_mark(offset+xbar*(menu_pos), ybar, MARKRECT);
    #endif
  }
  if(!pressed && menu_pos == 1)
  {
    buffer_flag = 1;
    buffering ++;
    if(buffering>1) buffering = 0;
  }
  if(!pressed && menu_pos == 2)
  {
    scroll_flag_y = 1;
    buff_scroll_y--;
    if(buff_scroll_y<=SCROLL_Y_MIN||buff_scroll_y>=SCROLL_Y_MAX)
    {
      buff_scroll_y = SCROLL_Y_MIN;
    }
  }
  if(!pressed && menu_pos == 3)
  {
    scroll_flag_x = 1;
    buff_scroll_x-=10;
    if(buff_scroll_x<=1||buff_scroll_x>tft_W)
    {
      buff_scroll_x=0;
    }
  }
  if(!pressed && menu_pos == 4)
  {
    speed_flag = 1;
    // adc_speed /= 2;
    adc_speed = adc_speed >> 1;
    spped_point -= 8;
    if(adc_speed<=SPEED_MIN||adc_speed>=SPEED_MAX)
    {
      adc_speed=SPEED_MIN;
      spped_point=8;
    }
  }
  if(!pressed && menu_pos ==5)
  {
    trigger_flag_hi = 1;
    trigger-=100;
    if(trigger<=1||trigger>=3300)
    {
      trigger=0;
    }
  }
  if(!pressed && menu_pos == 6)
  {
    trigger_flag_lo = 1;
    trigger-=100;
    if(trigger<=1||trigger>=3300)
    {
      trigger=0;
    }
  }
  if(!pressed && menu_pos == 7)
  {
    gain_flag = 1;
    gain--;
    if(gain<=1||gain>=8)
    {
      gain=1;
    }
  }
  if(!pressed && menu_pos == 8)
  {
    chanal_flag = 1;
    chanal++;
    if(chanal>1)chanal=0;
    // --
    if(!chanal)
    { 
      chanal_a = 1;
      chanal_b = 0;
    }
    if(chanal)
    { 
      chanal_a = 0;
      chanal_b = 1;
    }
  }
  oscil_stop = 0;
}

void encoder_faling(void)
{
  oscil_stop = 1;
  if(pressed)
  {
    menu_pos ++;
    if( menu_pos > 7) menu_pos = 8;
    #if debug_position
      write_enc_val(380, tft_H-18, menu_pos);
      write_val_mark(offset+xbar*(menu_pos), ybar, MARKRECT);
    #endif
  }
  if(!pressed && menu_pos == 1)
  {
    buffer_flag = 1;
    buffering ++;
    if(buffering>1) buffering = 0;
  }
  if(!pressed && menu_pos == 2)
  {
    scroll_flag_y = 1;
    buff_scroll_y++;
    if(buff_scroll_y<=SCROLL_Y_MIN||buff_scroll_y>=SCROLL_Y_MAX)
    {
      buff_scroll_y = SCROLL_Y_MAX;
    }
  }
  if(!pressed && menu_pos == 3)
  {
    scroll_flag_x = 1;
    buff_scroll_x+=10;
    if(buff_scroll_x<=1||buff_scroll_x>=tft_W)
    {
      buff_scroll_x=tft_W;
    }
  }
  if(!pressed && menu_pos == 4)
  {
    speed_flag = 1;
    // adc_speed *= 2;
    adc_speed = adc_speed << 1;
    spped_point += 8;
    if(adc_speed<=SPEED_MIN||adc_speed>=SPEED_MAX)
    {
      adc_speed=SPEED_MAX;
      spped_point=96;
    }
  }
  if(!pressed && menu_pos == 5)
  {
    trigger_flag_hi = 1;
    trigger+=100;
    if(trigger<=1||trigger>=3300)
    {
      trigger=3300;
    }
  }
  if(!pressed && menu_pos == 6)
  {
    trigger_flag_lo = 1;
    trigger+=100;
    if(trigger<=1||trigger>=3300)
    {
      trigger=3300;
    }
  }
  if(!pressed && menu_pos == 7)
  {
    gain_flag = 1;
    gain++;
    if(gain<=1||gain>=8)
    {
      gain=8;
    }
  }
  if(!pressed && menu_pos == 8)
  {
    chanal_flag = 1;
    chanal++;
    if(chanal>1)chanal=0;
    // --
    if(!chanal)
    { 
      chanal_a = 1;
      chanal_b = 0;
    }
    if(chanal)
    { 
      chanal_a = 0;
      chanal_b = 1;
    }
  }
  oscil_stop = 0;
}

void encoder_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void encoder_re_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
* @brief encoder connected to pins: PB3 and PB4, pin PA15 button switch
* @param None
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  delay_enc(100); // anti-tinkling
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  HAL_NVIC_DisableIRQ(EXTI3_IRQn);
  encoder_re_init();

  if(GPIO_Pin == GPIO_PIN_15) // This means we're sure we've gotten an EXTI event on pin 15
  {
    if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)) buttons();
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)) tft_fill_grid(24/*y1*/, 0/*x1*/, 240-24/*y2*/, tft_W/*x2*/, COLOR_BLACK);
  }
  if(GPIO_Pin == GPIO_PIN_3) // This means we're sure we've gotten an EXTI event on pin 3
  {
    if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) && !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) encoder_rising();
    if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) encoder_faling();
  }

  menu_upgrade();
  encoder_init();
}

