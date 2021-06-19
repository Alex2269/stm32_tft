#ifndef __ftf_defines_H
#define __ftf_defines_H

/* Define for connection */

#define 	RS_PORT		DSP_RS_GPIO_Port
#define		RS_PIN		DSP_RS_Pin

#define		WR_PORT		DSP_WR_GPIO_Port
#define 	WR_PIN		DSP_WR_Pin

#define		RD_PORT		DSP_RD_GPIO_Port
#define		RD_PIN		DSP_RD_Pin

#define		CS_PORT		DSP_CS_GPIO_Port
#define		CS_PIN		DSP_CS_Pin

#define		REST_PORT	DSP_RST_GPIO_Port
#define		REST_PIN	DSP_RST_Pin

#define		RST_PORT	DSP_RST_GPIO_Port
#define		RST_PIN		DSP_RST_Pin

#define		D0_PORT		DSP_0_GPIO_Port
#define		D0_PIN		DSP_0_Pin

#define		D1_PORT		DSP_1_GPIO_Port
#define		D1_PIN		DSP_1_Pin

#define		D2_PORT		DSP_2_GPIO_Port
#define		D2_PIN		DSP_2_Pin

#define		D3_PORT		DSP_3_GPIO_Port
#define		D3_PIN		DSP_3_Pin

#define		D4_PORT		DSP_4_GPIO_Port
#define		D4_PIN		DSP_4_Pin

#define		D5_PORT		DSP_5_GPIO_Port
#define		D5_PIN		DSP_5_Pin

#define		D6_PORT		DSP_6_GPIO_Port
#define		D6_PIN		DSP_6_Pin

#define		D7_PORT		DSP_7_GPIO_Port
#define		D7_PIN		DSP_7_Pin

#define		D8_PORT		DSP_8_GPIO_Port
#define		D8_PIN		DSP_8_Pin

#define		D9_PORT		DSP_9_GPIO_Port
#define		D9_PIN		DSP_9_Pin

#define		D10_PORT	DSP_10_GPIO_Port
#define		D10_PIN		DSP_10_Pin

#define		D11_PORT	DSP_11_GPIO_Port
#define		D11_PIN		DSP_11_Pin

#define		D12_PORT	DSP_12_GPIO_Port
#define		D12_PIN		DSP_12_Pin

#define		D13_PORT	DSP_13_GPIO_Port
#define		D13_PIN		DSP_13_Pin

#define		D14_PORT	DSP_14_GPIO_Port
#define		D14_PIN		DSP_14_Pin

#define		D15_PORT	DSP_15_GPIO_Port
#define		D15_PIN		DSP_15_Pin

/* Declare Private Macro */
#define		pin_low(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET)
#define		pin_high(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET)

#define tft_write_reg_data(reg, data)	{ tft_write_reg(reg); tft_write_data(data); }

#define		max(a,b)		(a>b)?a:b
#define		min(a,b)		(a<b)?a:b
#define		abs(a)			(a>=0)?a:(-a)

#define RD	5
#define WR	6
#define RS	7
#define CS	8
#define RST	9

#define GPIOA_Hi    (*(volatile unsigned long*)(GPIOA_BASE + 0x10))
#define GPIOA_Lo    (*(volatile unsigned long*)(GPIOA_BASE + 0x14))

#define GPIOB_Hi    (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_Lo    (*(volatile unsigned long*)(GPIOB_BASE + 0x14))

/* Declare TFT Dimension */

#define COLOR_WHITE			0xFFFF
#define COLOR_BLACK			0x0000
#define COLOR_BLUE			0x001F
#define COLOR_BRED			0xF81F
#define COLOR_GRED			0xFFE0
#define COLOR_GBLUE			0x07FF
#define COLOR_RED			0xF800
#define COLOR_VIOLET		0xF81F
#define COLOR_GREEN			0x07E0
#define COLOR_CYAN			0x7FFF
#define COLOR_YELLOW		0xFFE0
#define COLOR_BROWN			0xBC40
#define COLOR_BRRED			0xFC07
#define COLOR_GRAY			0x8430
#define COLOR_DARKBLUE		0x01CF
#define COLOR_LIGHTBLUE		0x7D7C
#define COLOR_GRAYBLUE		0x5458
#define COLOR_LIGHTGREEN	0x841F
#define COLOR_LGRAY			0xC618
#define COLOR_LGRAYBLUE		0xA651
#define COLOR_LBBLUE		0x2B12

/* Define TFT String Mode */
#define TFT_STRING_MODE_NO_BACKGROUND	0x01
#define TFT_STRING_MODE_BACKGROUND		0x00

//-------------begin------------------------------
// general purpose pin macros

#define RD_ACTIVE     pin_write(DSP_RD_GPIO_Port, DSP_RD_Pin, GPIO_PIN_RESET)
#define RD_IDLE       pin_write(DSP_RD_GPIO_Port, DSP_RD_Pin, GPIO_PIN_SET)

#define WR_ACTIVE     pin_write(DSP_WR_GPIO_Port, DSP_WR_Pin, GPIO_PIN_RESET)
#define WR_IDLE       pin_write(DSP_WR_GPIO_Port, DSP_WR_Pin, GPIO_PIN_SET)

#define CS_ACTIVE     pin_write(DSP_CS_GPIO_Port, DSP_CS_Pin, GPIO_PIN_RESET)
#define CS_IDLE       pin_write(DSP_CS_GPIO_Port, DSP_CS_Pin, GPIO_PIN_SET)

#define RESET_ACTIVE  pin_write(DSP_RST_GPIO_Port, DSP_RST_Pin, GPIO_PIN_RESET)
#define RESET_IDLE    pin_write(DSP_RST_GPIO_Port, DSP_RST_Pin, GPIO_PIN_SET)

#define DC_ACTIVE  pin_write(DSP_RS_GPIO_Port, DSP_RS_Pin, GPIO_PIN_RESET)
#define DC_IDLE    pin_write(DSP_RS_GPIO_Port, DSP_RS_Pin, GPIO_PIN_SET)

// General macros.   IOCLR registers are 1 cycle when optimised.
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }         //PWLW=TWRL=50ns
#define RD_STROBE RD_IDLE, RD_ACTIVE, RD_ACTIVE, RD_ACTIVE   //PWLR=TRDL=150ns

//------------------------------------------------

#define delay(x)     HAL_Delay(x)
#define swap(a, b)    { uint16_t  t=a; a=b; b=t; }

//-------------end--------------------------------

#endif // __ftf_defines_H
