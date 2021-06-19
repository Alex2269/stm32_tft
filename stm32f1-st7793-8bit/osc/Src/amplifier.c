#include "amplifier.h"

/*
 * select the gain
 */
uint8_t spi_amplifier_gain(uint8_t gain)
{
  static uint8_t amplification = 0;
  static uint8_t G = 0;

  if(gain == 1)
  {
    amplification = 0b000; // gain x1
    G = 1;
  }
  if(gain == 2)
  {
    amplification = 0b001; // gain x2
    G = 2;
  }
  if(gain == 3)
  {
    amplification = 0b010; // gain x4
    G = 4;
  }
  if(gain == 4)
  {
    amplification = 0b011; // gain x5
    G = 5;
  }
  if(gain == 5)
  {
    amplification = 0b100; // gain x8
    G = 8;
  }
  if(gain == 6)
  {
    amplification = 0b101; // gain x10
    G = 10;
  }
  if(gain == 7)
  {
    amplification = 0b110; // gain x16
    G = 16;
  }
  if(gain == 8)
  {
    amplification = 0b111; // gain x32
    G = 32;
  }

  *(amplifier_cs)= 0; // select chip
  soft_spi_wr(0b01000000); // send GAIN command
  soft_spi_wr(amplification); // transfer data
  *(amplifier_cs)= 1; // end transmission
  return G; // Gain
}

/*
 * select the channel
 * channel accepts values 0 or 1
 */
void spi_amplifier_channel(uint8_t channel)
{
  *(amplifier_cs)= 0; // select chip
  soft_spi_wr(0b01000001); // send CHANNEL command
  soft_spi_wr(channel); // send address
  *(amplifier_cs)= 1; // end transmission
}

/*
 * no use sleep mode
 */
void spi_amplifier_sleep(void)
{
  *(amplifier_cs)= 0; // select chip
  soft_spi_wr(0b00100000); // send SLEEP command
  soft_spi_wr(0x00); // send address
  *(amplifier_cs)= 1; // end transmission
}

void spi_amplifier(uint8_t gain, uint8_t channel)
{
  spi_amplifier_gain(gain);
  spi_amplifier_channel(channel);
}

