#ifndef __bit_band_H
#define __bit_band_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* bitband type */
typedef volatile uint32_t * bitband_t;

/* base address for bit banding */
#define BITBAND_SRAM_REF              (0x20000000)
/* base address for bit banding */
#define BITBAND_SRAM_BASE             (0x22000000)
/* base address for bit banding */
#define BITBAND_PERIPH_REF            (0x40000000)
/* base address for bit banding */
#define BITBAND_PERIPH_BASE           (0x42000000)

/* sram bit band */
#define BITBAND_SRAM(address, bit)     ((void*)(BITBAND_SRAM_BASE +   \
                (((uint32_t)address) - BITBAND_SRAM_REF) * 32 + (bit) * 4))

/* periph bit band */
#define BITBAND_PERIPH(address, bit)   ((void *)(BITBAND_PERIPH_BASE + \
                (((uint32_t)address) - BITBAND_PERIPH_REF) * 32 + (bit) * 4))


#ifdef __cplusplus
}
#endif

#endif /* __bit_band_H */