
``` c

    configure pins:
    Core/hardware/io_ILI9325.h

    connected mcu -> tft 
       ──────
    ──│      ├── PB0 ─── LCD_WR ─── Y(+)
    ──│      ├── PB1 ─── LCD_DC ─── X(-)
    ──│      ├── PB2 ─── LCD_D0
    ──│      ├── PB3 ─── LCD_D1
    ──│      ├── PB4 ─── LCD_D2
    ──│      ├── PB5 ─── LCD_D3
    ──│      ├── PB6 ─── LCD_D4
    ──│      ├── PB7 ─── LCD_D5
    ──│      ├── PB8 ─── LCD_D6 ─── X(+)
    ──│      ├── PB9 ─── LCD_D7 ─── Y(-)
    ──│      ├── PB13 ── LCD_RD
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

    #define WR 0
    #define DC 1
    #define RD 13
    #define CS 14
    #define RST 15

    void tft_write_reg(uint8_t eightbit);
    void tft_write_data(uint8_t eightbit);
```
