#ifndef __PGMSPACE_H_
#define __PGMSPACE_H_ 1

#include <inttypes.h>

#define PROGMEM
#define PGM_P  const char *
#define PSTR(str) (str)

#define _SFR_BYTE(n) (n)

typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef int8_t prog_int8_t;
typedef uint8_t prog_uint8_t;
typedef int16_t prog_int16_t;
typedef uint16_t prog_uint16_t;
typedef int32_t prog_int32_t;
typedef uint32_t prog_uint32_t;

#define memcpy_P(dest, src, num) memcpy((dest), (src), (num))
#define strcpy_P(dest, src) strcpy((dest), (src))
#define strcat_P(dest, src) strcat((dest), (src))
#define strcmp_P(a, b) strcmp((a), (b))
#define strstr_P(a, b) strstr((a), (b))
#define strlen_P(a) strlen((a))
#define sprintf_P(s, f, ...) sprintf((s), (f), __VA_ARGS__)

#ifndef PROGMEM
 #define PROGMEM(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#ifndef pgm_read_float
 #define pgm_read_float(addr) (*(const float *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

//#ifndef min
//  #define min(a,b) (((a) < (b)) ? (a) : (b))
//#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#ifndef _swap_int32_t
#define _swap_int32_t(a, b) { int32_t t = a; a = b; b = t; }
#endif

#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#define pgm_read_word_near(addr) pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr) pgm_read_byte(addr)
#define pgm_read_word_far(addr) pgm_read_word(addr)
#define pgm_read_dword_far(addr) pgm_read_dword(addr)
#define pgm_read_float_far(addr) pgm_read_float(addr)

#endif
