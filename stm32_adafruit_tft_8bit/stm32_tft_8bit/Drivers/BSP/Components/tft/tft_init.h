#ifndef init_R6158_H
#define init_R6158_H

/* Include Library */
#include "stdint.h"

#define TFTLCD_DELAY8 0xFF
#define TFTLCD_DELAY 0x00FF

const uint8_t init_array_hx8357_8[] =
{
  //cmd number-data data's
  0x01, 0, //Software Reset
  TFTLCD_DELAY8, 10, // will power up with ONLY reset, sleep out, display on
  0x11, 0, // Sleep OUT
  0x3A, 1, 0x55, //Pixel read=565, write=565.
  TFTLCD_DELAY8, 150,

  0x20, 0, // INVOFF 0x20
  //x21, 0, // INVON 0x21
  //tft_SetRotation(0);
  0x29, 0, //Display On
};

const uint8_t init_array_R61581_8[] =
{
  //cmd number-data data's
  0x01, 0, //Soft Reset
  TFTLCD_DELAY8, 150, // will power up with ONLY reset, sleep out, display on
  0x28, 0, //Display Off
  0x3A, 1, 0x55, //Pixel read=565, write=565.

  // Atmel MaxTouch
  0xB0, 1, 0x00, // unlocks E0, F0
  0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
  0xB4, 1, 0x00, // Frame mode [00]
  0xD0, 3, 0x07, 0x42, 0x18, // Set Power [00 43 18] x1.00, x6, x3
  0xD1, 3, 0x00, 0x07, 0x10, // Set VCOM [00 00 00] x0.72, x1.02
  0xD2, 2, 0x01, 0x02, // Set Power for Normal Mode [01 22]
  0xD3, 2, 0x01, 0x02, // Set Power for Partial Mode [01 22]
  0xD4, 2, 0x01, 0x02, // Set Power for Idle Mode [01 22]
  0xC0, 5, 0x12, 0x3B, 0x00, 0x02, 0x11, //Panel Driving BGR for 1581 [10 3B 00 02 11]
  0xC5, 1, 0x03, //Frame Rate [03]

  0x11, 0, //Sleep Out
  TFTLCD_DELAY8, 150,
  0x29, 0, //Display On
};

const uint16_t init_array_R61581_16[] =
{
  //cmd number-data data's
  0x0001, 0, //Soft Reset
  TFTLCD_DELAY, 150, // will power up with ONLY reset, sleep out, display on
  0x0028, 0, //Display Off
  0x003A, 1, 0x0055, //Pixel read=565, write=565.

  // Atmel MaxTouch
  0x00B0, 1, 0x0000, // unlocks E0, F0
  0x00B3, 4, 0x0002, 0x0000, 0x0000, 0x0000, //Frame Memory, interface [02 00 00 00]
  0x00B4, 1, 0x0000, // Frame mode [00]
  0x00D0, 3, 0x0007, 0x0042, 0x0018, // Set Power [00 43 18] x1.00, x6, x3
  0x00D1, 3, 0x0000, 0x0007, 0x0010, // Set VCOM [00 00 00] x0.72, x1.02
  0x00D2, 2, 0x0001, 0x0002, // Set Power for Normal Mode [01 22]
  0x00D3, 2, 0x0001, 0x0002, // Set Power for Partial Mode [01 22]
  0x00D4, 2, 0x0001, 0x0002, // Set Power for Idle Mode [01 22]
  0x00C0, 5, 0x0012, 0x003B, 0x0000, 0x0002, 0x0011, //Panel Driving BGR for 1581 [10 3B 00 02 11]
  0x00C5, 1, 0x0003, //Frame Rate [03]

  0x0011, 0, //Sleep Out
  TFTLCD_DELAY, 150,
  0x0029, 0, //Display On
};

const uint16_t init_array_ili9325_16[] =
{
  0x00E5, 1, 0x78F0,     // set SRAM internal timing
  0x0001, 1, 0x0100,     // set Driver Output Control
  0x0002, 1, 0x0200,     // set 1 line inversion
  0x0003, 1, 0x1030,     // set GRAM write direction and BGR=1.
  0x0004, 1, 0x0000,     // Resize register
  0x0005, 1, 0x0000,     // .kbv 16bits Data Format Selection
  0x0008, 1, 0x0207,     // set the back porch and front porch
  0x0009, 1, 0x0000,     // set non-display area refresh cycle ISC[3:0]
  0x000A, 1, 0x0000,     // FMARK function
  0x000C, 1, 0x0000,     // RGB interface setting
  0x000D, 1, 0x0000,     // Frame marker Position
  0x000F, 1, 0x0000,     // RGB interface polarity
  // ----------- Power On sequence ----------- //
  0x0010, 1, 0x0000,     // SAP, BT[3:0], AP, DSTB, SLP, STB
  0x0011, 1, 0x0007,     // DC1[2:0], DC0[2:0], VC[2:0]
  0x0012, 1, 0x0000,     // VREG1OUT voltage
  0x0013, 1, 0x0000,     // VDV[4:0] for VCOM amplitude
  0x0007, 1, 0x0001,
  TFTLCD_DELAY, 200,  // Dis-charge capacitor power voltage
  0x0010, 1, 0x1690,     // SAP=1, BT=6, APE=1, AP=1, DSTB=0, SLP=0, STB=0
  0x0011, 1, 0x0227,     // DC1=2, DC0=2, VC=7
  TFTLCD_DELAY, 50,   // wait_ms 50ms
  0x0012, 1, 0x000D,     // VCIRE=1, PON=0, VRH=5
  TFTLCD_DELAY, 50,   // wait_ms 50ms
  0x0013, 1, 0x1200,     // VDV=28 for VCOM amplitude
  0x0029, 1, 0x000A,     // VCM=10 for VCOMH
  0x002B, 1, 0x000D,     // Set Frame Rate
  TFTLCD_DELAY, 50,   // wait_ms 50ms
  0x0020, 1, 0x0000,     // GRAM horizontal Address
  0x0021, 1, 0x0000,     // GRAM Vertical Address

  // ----------- Adjust the Gamma Curve ----------//
  0x0030, 1, 0x0000,
  0x0031, 1, 0x0404,
  0x0032, 1, 0x0003,
  0x0035, 1, 0x0405,
  0x0036, 1, 0x0808,
  0x0037, 1, 0x0407,
  0x0038, 1, 0x0303,
  0x0039, 1, 0x0707,
  0x003C, 1, 0x0504,
  0x003D, 1, 0x0808,

  //------------------ Set GRAM area ---------------//
  0x0060, 1, 0x2700,     // Gate Scan Line GS=0 [0xA700] 
  0x0061, 1, 0x0001,     // NDL,VLE, REV .kbv
  0x006A, 1, 0x0000,     // set scrolling line
  //-------------- Partial Display Control ---------//
  0x0080, 1, 0x0000,
  0x0081, 1, 0x0000,
  0x0082, 1, 0x0000,
  0x0083, 1, 0x0000,
  0x0084, 1, 0x0000,
  0x0085, 1, 0x0000,
  //-------------- Panel Control -------------------//
  0x0090, 1, 0x0010,
  0x0092, 1, 0x0000,
  0x0007, 1, 0x0133,     // 262K color and display ON
};


const uint16_t ST7793_regValues[] =
{
  0x0000, 1, 0x0000,
  0x0000, 1, 0x0000,
  0x0000, 1, 0x0000,
  0x0000, 1, 0x0000,
  //TFTLCD_DELAY, 15,
  0x0400, 1, 0x6200, // NL=0x31 (49) i.e. 400 rows
  0x0008, 1, 0x0808,
  // gamma
  0x0300, 1, 0x0C00,
  0x0301, 1, 0x5A0B,
  0x0302, 1, 0x0906,
  0x0303, 1, 0x1017,
  0x0304, 1, 0x2300,
  0x0305, 1, 0x1700,
  0x0306, 1, 0x6309,
  0x0307, 1, 0x0C09,
  0x0308, 1, 0x100C,
  0x0309, 1, 0x2232,
  0x0010, 1, 0x0016, // 69.5Hz 0016
  0x0011, 1, 0x0101,
  0x0012, 1, 0x0000,
  0x0013, 1, 0x0001,
  0x0100, 1, 0x0330, // BT,AP
  0x0101, 1, 0x0237, // DC0,DC1,VC
  0x0103, 1, 0x0D00, // VDV
  0x0280, 1, 0x6100, // VCM
  0x0102, 1, 0xC1B0, // VRH,VCMR,PSON,PON
  //TFTLCD_DELAY, 50,
  0x0001, 1, 0x0100,
  0x0002, 1, 0x0100,
  0x0003, 1, 0x1030, // 1030
  0x0009, 1, 0x0001,
  0x000C, 1, 0x0000,
  0x0090, 1, 0x8000,
  0x000F, 1, 0x0000,
  0x0210, 1, 0x0000,
  0x0211, 1, 0x00EF,
  0x0212, 1, 0x0000,
  0x0213, 1, 0x018F, // 432=01AF,400=018F
  0x0500, 1, 0x0000,
  0x0501, 1, 0x0000,
  0x0502, 1, 0x005F, // ???
  0x0401, 1, 0x0001, // REV=1
  0x0404, 1, 0x0000,
  //TFTLCD_DELAY, 50,
  0x0007, 1, 0x0100, // BASEE
  //TFTLCD_DELAY, 50,
  0x0200, 1, 0x0000,
  0x0201, 1, 0x0000,
};

const uint16_t test_array[] =
{
  0x0008, 1, 0x0808,
};

#endif // init_R6158_H
