#ifndef __MENU_H
#define __MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include "main.h"

#include "time.h"
#include "soft_spi_ts.h"

typedef struct
{
  uint16_t X, Y;
  uint16_t W, H;
  uint16_t Color;
  uint8_t *text;
  uint8_t *name;
  uint8_t active;
}push_button;

void menu_init(void);
void write_info_touch(void);
void call_button_yell(void);
void call_button_red(void);
void draw_button(push_button* pb, uint16_t* image, uint8_t* name, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color, uint8_t* text);
uint8_t get_button(push_button* pb);

#endif // __MENU_H
