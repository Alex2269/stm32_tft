
``` c

    SSD1963 8-bit mode
    configure pins:
    hardware/io_SSD1963.h

    connected mcu -> tft 
       ──────
    ──│      ├── PB2 ─── LCD_D0
    ──│      ├── PB3 ─── LCD_D1
    ──│      ├── PB4 ─── LCD_D2
    ──│      ├── PB5 ─── LCD_D3
    ──│      ├── PB6 ─── LCD_D4
    ──│      ├── PB7 ─── LCD_D5
    ──│      ├── PB8 ─── LCD_D6
    ──│      ├── PB9 ─── LCD_D7
    ──│      ├── PB10 ── LCD_RD
    ──│      ├── PB12 ── LCD_WR
    ──│      ├── PB13 ── LCD_RS
    ──│      ├── PB14 ── LCD_CS
    ──│      ├── PB15 ── LCD_RST
       ──────

    #define D0 2
    #define D1 3
    #define D2 4
    #define D3 5
    #define D4 6
    #define D5 7
    #define D6 8
    #define D7 9

    #define RD 10
    #define WR 12
    #define DC 13
    #define CS 14
    #define RST 15

    void tft_write_reg(uint8_t eightbit);
    void tft_write_data(uint8_t eightbit);
```
