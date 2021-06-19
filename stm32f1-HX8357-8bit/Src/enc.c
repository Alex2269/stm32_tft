#include "osc.h"
#include "main.h"
#include "enc.h"

extern bool oscil_stop;
extern uint16_t buff_scroll;
extern uint16_t trigger;
extern uint16_t adc_speed;
static uint16_t spped_point;

char label_scrool[8] = "scrollX";
char label_trigger[8] = "trigger";
char label_speed[8] = ">speed<";

static uint16_t button = 1;

void buttons(void)
{
  for(uint16_t i = 0; i<5000; i++)__asm volatile ("nop"); // anti-tinkling
  {
    button++;
    if(button>3)button=1;
  }
  // --
  if(button == 1)
  {
    write_string(320, 0, (int8_t*)label_trigger);
    write_value(210,0, trigger);
  }
  // --
  if(button == 2)
  {
    write_string(320, 0, (int8_t*)label_speed);
    write_value(210,0, adc_speed);
  }
  // --
  if(button == 3)
  {
    write_string(320, 0, (int8_t*)label_scrool);
    write_value(210,0, buff_scroll);
  }
  // --
  tft_fill(38,1,tft_H-38,tft_W-1,COLOR_BLACK); // cleaning of display
  clean_ray();
  write_grid();
  oscil_stop = 0;
}

// --
void encoder_rising(void)
{
  if(button == 1)
  {
    trigger-=100;
    if(trigger<100)trigger=3300;
    if(trigger>3300)trigger=0;
    if((trigger>100)&&(trigger<3100))write_marker_point(10, tft_H-32-trigger/12, 1);
    // tft_SetRotation(0);
    // write_value(140,0, trigger);
    // tft_SetRotation(1);
  }
  // --
  if(button == 2)
  {
    adc_speed /= 2;
    spped_point -= 8;
    if(adc_speed<1)spped_point=96;
    if(adc_speed>1024)spped_point=8;
    if(adc_speed<1)adc_speed=1024;
    if(adc_speed>1024)adc_speed=1;
    write_marker_point(310+spped_point, 25, 1);
    // write_string(320,0, (int8_t*)label_speed);
    // write_value(210,0, adc_speed);
  }
  // --
  if(button == 3)
  {
    buff_scroll-=20;
    if(buff_scroll<20)buff_scroll=0;
    // if(buff_scroll>tft_W)buff_scroll=0;
    if(buff_scroll>tft_W)buff_scroll=tft_W;
    write_marker_point(buff_scroll, 36, 1);
    // write_string(320, 0, (int8_t*)label_scrool);
    // write_value(210,0, buff_scroll);
  }
  // --
  // tft_fill(38,1,tft_H-38,tft_W-1,COLOR_BLACK); // cleaning of display
  // clean_ray();
  // write_grid();
  oscil_stop = 0;
}

// --
void encoder_faling(void)
{
  if(button == 1)
  {
    trigger+=100;
    if(trigger<100)trigger=3300;
    if(trigger>3300)trigger=0;
    if((trigger>100)&&(trigger<3100))write_marker_point(10, tft_H-32-trigger/12, 1);
    // tft_SetRotation(0);
    // write_value(140,0, trigger);
    // tft_SetRotation(1);
  }
  // --
  if(button == 2)
  {
    adc_speed *= 2;
    spped_point += 8;
    if(adc_speed<1)spped_point=96;
    if(adc_speed>1024)spped_point=8;
    if(adc_speed<1)adc_speed=1024;
    if(adc_speed>1024)adc_speed=1;
    write_marker_point(310+spped_point, 25, 1);
    // write_string(320,0, (int8_t*)label_speed);
    // write_value(210,0, adc_speed);
  }
  // --
  if(button == 3)
  {
    buff_scroll+=20;
    if(buff_scroll<20)buff_scroll=0;
    // if(buff_scroll>tft_W)buff_scroll=tft_W;
    if(buff_scroll>tft_W)buff_scroll=0;
    write_marker_point(buff_scroll, 36, 1);
    // write_string(320, 0, (int8_t*)label_scrool);
    // write_value(210,0, buff_scroll);
  }
  // --
  // tft_fill(38,1,tft_H-38,tft_W-1,COLOR_BLACK); // cleaning of display
  // clean_ray();
  // write_grid();
  oscil_stop = 0;
}

/**
* @brief encoder connected to pins: PB3 and PB4, pin PA15 button switch
* @param None
* @retval None
***/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  oscil_stop = 1;
  if(GPIO_Pin == GPIO_PIN_15) // This means we're sure we've gotten an EXTI event on pin 15
  {
    buttons();
  }
  // --
  if(GPIO_Pin == GPIO_PIN_3) // This means we're sure we've gotten an EXTI event on pin 3
  {
    for(uint16_t i = 0; i<100; i++)__asm volatile ("nop"); // anti-tinkling
    // rotate encoder
    if(1 == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) // While rising edge on PB4.
    {
      encoder_rising();
    }
    // --
    // rotate encoder
    if(0 == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) // PB4 when the falling edge is occurring.
    {
      encoder_faling();
    }
    // --
  }
  // --
  else
  {
    __asm volatile ("nop"); // anti-tinkling
  }
  // --
}

// --
