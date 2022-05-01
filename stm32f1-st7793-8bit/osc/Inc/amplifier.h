#ifndef _amplifier_H
#define _amplifier_H

#include "stdint.h"
#include "pins_ext.h"
#include "bit_band.h"
#include "soft_spi.h"

bitband_t amplifier_cs;

uint8_t spi_amplifier_gain(uint8_t gain);
void spi_amplifier_channel(uint8_t channel);
void spi_amplifier_sleep(void);
void spi_amplifier(uint8_t gain, uint8_t channel);

#endif // _amplifier_H

