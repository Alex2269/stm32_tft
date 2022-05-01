#ifndef __FTOA_IMPL_H
#define __FTOA_IMPL_H

// C program for implementation of ftoa()
#include "ftoa_impl.h"
#include <math.h>
#include <stdint.h>

// Reverses a string 'str' of length 'len'
void reverse(uint8_t* str, uint8_t len)
{
  int8_t i = 0, j = len - 1, temp;
  while (i < j)
  {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
uint32_t intToStr(uint32_t x, uint8_t str[], uint8_t d)
{
  uint8_t i = 0;
  while (x)
  {
    str[i++] = (x % 10) + '0';
    x = x / 10;
  }

  // If number of digits required is more, then
  // add 0s at the beginning
  while (i < d)
  str[i++] = '0';
  reverse(str, i);
  str[i] = '\0';
  return i;
}

// Converts a floating-point/double number to a string.
void ftoa_local(float n, uint8_t* res, uint8_t afterpoint)
{
  // Extract integer part
  uint32_t ipart = (uint32_t)n;

  // Extract floating part
  float fpart = n - (float)ipart;

  // convert integer part to string
  uint32_t i = intToStr(ipart, res, 0);

  // check for display option after point
  if (afterpoint != 0)
  {
    res[i] = '.'; // add dot

    // Get the value of fraction part upto given no.
    // of points after dot. The third parameter
    // is needed to handle cases like 233.007
    fpart = fpart * pow(10, afterpoint);
    intToStr((uint32_t)fpart, res + i + 1, afterpoint);
  }
}

uint8_t utoa_local(int64_t value, uint8_t *sp, int64_t radix)
{
  uint8_t len;
  uint8_t tmp[16], *tp = tmp;
  int64_t i, v, sign = radix == 10 && value < 0;
  v = sign ? -value : value;

  while (v || tp == tmp)
  {
    i = v % radix;
    v /= radix;
    *tp++ = i < 10 ? (uint8_t) (i + '0') : (uint8_t) (i + 'a' - 10);
  }

  len = tp - tmp;

  if (sign)
  {
    *sp++ = '-';
    len++;
  }

  while (tp > tmp)
  *sp++ = *--tp;
  *sp++ = '\0';
  return len;
}

#endif // __FTOA_IMPL_H
