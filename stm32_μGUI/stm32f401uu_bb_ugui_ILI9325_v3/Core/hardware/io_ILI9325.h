/*
 * 
 * io_ILI9325.h
 *
 */

#ifndef io_ILI9325_H_
#define io_ILI9325_H_

#include "ugui_ILI9325.h"

#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

#define WR 0
#define DC 1
#define RD 13
#define CS 14
#define RST 15
    
/********************************************************************//**
* @brief Bit-Banding Macro Functions
**********************************************************************/
/******************************************************************************/
/*                      Bit Banding declarations */
/******************************************************************************/
/* The processor memory map includes two bit-band regions. These occupy the lowest
* 1MB of the SRAM and peripheral memory regions respectively.
* + SRAM: Bit-band region:      0x20000000 - 0x200FFFFF
*   Bit-band alias:             0x22000000 - 0x23FFFFFF
* + PERI: Bit-band region:      0x40000000 - 0x400FFFFF
*   Bit-band alias:             0x42000000 - 0x43FFFFFF
* The mapping formula:
* bit_word_offset = (byte_offset * 32) + (bit_number * 4)
* bit_word_address = bit_band_base + byte_offset
* where:
* + bit_word_offset: the position of the target bit in the bit-band memory region
* + bit_word_addr: the address of the word in the alias memory region that maps to the target bit
* + bit_band_base: the starting address of the alias region
* + byte_offset: the number of byte in the bit-band region that contains the targeted bit
* + bit_number: is the bit position (0-7) of the targeted bit
*
* Note:
* The SRAM bit band accessible area on STM32F407 is available in two ranges:
* + 0x20000000 - 0x2001BFFF: for SRAM1 Base
* + 0x2001C000 - 0x2001FFFC: for SRAM2 Base
*
* The Peripheral bit band accessible are on STM32F407 is available in three range:
* + 0x40000000 - 0x40007FFC : APB1
* + 0x40010000 - 0x40057fff : APB2
* + 0x40020000 - 0x4007FFFC : AHB1
*
*/

/** Peripheral bit band region address calculation */

#define PortA GPIOA_BASE+0x14
#define PortB GPIOB_BASE+0x14
#define PortC GPIOC_BASE+0x14
#define PortD GPIOD_BASE+0x14

/** SRAM bit band region address calculation */
#define BITBAND_SRAM(VarAddr,bitnumber) ((SRAM_BB_BASE + ((VarAddr-SRAM_BASE)<<5) + (bitnumber<<2)))
/* Basic bit band function definitions */
#define BITBAND_SRAM_ClearBit(VarAddr,bitnumber)(*(volatile uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)) = 0)
#define BITBAND_SRAM_SetBit(VarAddr,bitnumber) (*(volatile uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)) = 1)
#define BITBAND_SRAM_GetBit(VarAddr,bitnumber) (*(volatile uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)))

/** Peripheral bit band region address calculation */
#define BITBAND_Peri(VarAddr,bitnumber)((PERIPH_BB_BASE + ((VarAddr-PERIPH_BASE)<<5) + (bitnumber<<2)))
/* Basic bit band function definitions */
#define BITBAND_Peri_ClearBit(VarAddr,bitnumber)(*(volatile uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)) = 0)
#define BITBAND_Peri_SetBit(VarAddr,bitnumber) (*(volatile uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)) = 1)
#define BITBAND_Peri_GetBit(VarAddr,bitnumber) (*(volatile uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)))

/********************************************************************//**
* @brief Bit-Banding Functions
**********************************************************************/
/**
 * @brief This function Reads bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  Bit Value
 */
static inline
uint32_t Bitband_Getbit(uint32_t Address, uint8_t pin)
{
  uint32_t test = 0;
  if (Address >= 0x20000000 && Address <= 0x2001FFFC)
  {
    test = BITBAND_SRAM_GetBit(Address, pin);
  }
  else if((Address >= 0x40000000 && Address <= 0x40007FFC) ||
          (Address >= 0x40010000 && Address <= 0x40057fff) ||
          (Address >= 0x40020000 && Address <= 0x4007FFFC))
  {
    test = BITBAND_Peri_GetBit(Address, pin);
  }
  return test;
}

/**
 * @brief This function Sets bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  None
 */
static inline
void Bitband_Setbit(uint32_t Address, uint8_t pin)
{
  if (Address >= 0x20000000 && Address <= 0x2001FFFC)
  {
    BITBAND_SRAM_SetBit(Address, pin);
  }
  else if((Address >= 0x40000000 && Address <= 0x40007FFC) ||
          (Address >= 0x40010000 && Address <= 0x40057fff) ||
          (Address >= 0x40020000 && Address <= 0x4007FFFC))
  {
    BITBAND_Peri_SetBit(Address, pin);
  }
}

/**
 * @brief This function Clears bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  None
 */
static inline
void Bitband_clearbit(uint32_t Address, uint8_t pin)
{
  if (Address >= 0x20000000 && Address <= 0x2001FFFC)
  {
    BITBAND_SRAM_ClearBit(Address, pin);
  }
  else if((Address >= 0x40000000 && Address <= 0x40007FFC) ||
          (Address >= 0x40010000 && Address <= 0x40057fff) ||
          (Address >= 0x40020000 && Address <= 0x4007FFFC))
  {
    BITBAND_Peri_ClearBit(Address, pin);
  }
}

static inline
void write_pin(uint32_t port, uint8_t pin, bool state)
{
  if(state) BITBAND_Peri_SetBit(port, pin);
  else BITBAND_Peri_ClearBit(port,pin);
}

static inline
void write_bit(uint32_t Address, uint8_t pin, bool state)
{
  if(state) BITBAND_SRAM_SetBit(Address, pin);
  else     BITBAND_SRAM_ClearBit(Address, pin);
}

static inline
void tft_wr_pin_strob(void)
{
  BITBAND_Peri_ClearBit(PortB,WR);
  BITBAND_Peri_SetBit(PortB, WR);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
static inline
void tft_write_reg(uint8_t eightbit)
{
  // printf("GPIO_pin: com ");
  /* 
  write_pin(PortB,CS,0);
  write_pin(PortB,RD,1);
  write_pin(PortB,DC,0);

  write_pin(PortB,D0, (eightbit & 1) == 0 ? 0 : 1);
  write_pin(PortB,D1, (eightbit & 2) == 0 ? 0 : 1);
  write_pin(PortB,D2, (eightbit & 4) == 0 ? 0 : 1);
  write_pin(PortB,D3, (eightbit & 8) == 0 ? 0 : 1);
  write_pin(PortB,D4, (eightbit & 16) == 0 ? 0 : 1);
  write_pin(PortB,D5, (eightbit & 32) == 0 ? 0 : 1);
  write_pin(PortB,D6, (eightbit & 64) == 0 ? 0 : 1);
  write_pin(PortB,D7, (eightbit & 128) == 0 ? 0 : 1); */

  // GPIOB->ODR = (eightbit >> 2) | (eightbit << 6);
  // GPIOB->ODR = eightbit | (0<<CS)| (1<<RD) | (0<<DC);

  static uint16_t control = 0b0001110000000000 | (1<<RD) | (0<<WR) | (0<<DC) | (0<<CS) | (1<<RST);

  GPIOB->ODR = control | (eightbit<<2);
  // printf("eightbit: %X\n",eightbit);

  BITBAND_Peri_SetBit(PortB, WR);
  // tft_wr_pin_strob();
  // write_pin(PortB,CS,1);
}

static inline
void tft_write_data(uint8_t eightbit)
{
  // printf("GPIO_pin: data ");
  /*
  write_pin(PortB,CS,0);
  write_pin(PortB,RD,1);
  write_pin(PortB,DC,1);

  write_pin(PortB,D0, (eightbit & 1) == 0 ? 0 : 1);
  write_pin(PortB,D1, (eightbit & 2) == 0 ? 0 : 1);
  write_pin(PortB,D2, (eightbit & 4) == 0 ? 0 : 1);
  write_pin(PortB,D3, (eightbit & 8) == 0 ? 0 : 1);
  write_pin(PortB,D4, (eightbit & 16) == 0 ? 0 : 1);
  write_pin(PortB,D5, (eightbit & 32) == 0 ? 0 : 1);
  write_pin(PortB,D6, (eightbit & 64) == 0 ? 0 : 1);
  write_pin(PortB,D7, (eightbit & 128) == 0 ? 0 : 1); */

  static uint16_t control = 0b0001110000000000 | (1<<RD) | (0<<WR) | (1<<DC) | (0<<CS) | (1<<RST);

  GPIOB->ODR = control | (eightbit<<2);
  // printf("eightbit: %X\n",eightbit);

  BITBAND_Peri_SetBit(PortB, WR);
  // tft_wr_pin_strob();
  // write_pin(PortB,CS,1);
}
#pragma GCC pop_options

#endif // io_ILI9325_H_

/* [] END OF FILE */

