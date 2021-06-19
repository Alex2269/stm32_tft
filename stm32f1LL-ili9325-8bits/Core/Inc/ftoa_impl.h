#ifndef __FTOA_IMPL_H
#define __FTOA_IMPL_H

void reverse(uint8_t* str, uint8_t len);
uint32_t intToStr(uint32_t x, uint8_t str[], uint8_t d);
void ftoa_local(float n, uint8_t* res, uint8_t afterpoint);
uint8_t utoa_local(int64_t value, uint8_t *sp, int64_t radix);

#endif // __FTOA_IMPL_H
