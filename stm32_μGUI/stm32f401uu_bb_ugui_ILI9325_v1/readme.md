
``` c

    configure pins:
    Core/hardware/io_ILI9325.h

    connected mcu -> tft      -->    touch
       ──────
    ──│      ├── PB0 ─── LCD_D7 ─── Y(-)
    ──│      ├── PB1 ─── LCD_D6 ─── X(+)
    ──│      ├── PB2 ─── LCD_D5
    ──│      ├── PB3 ─── LCD_D4
    ──│      ├── PB4 ─── LCD_D3
    ──│      ├── PB5 ─── LCD_D2
    ──│      ├── PB6 ─── LCD_D1
    ──│      ├── PB7 ─── LCD_D0
    ──│      ├── PB10 ── LCD_RD
    ──│      ├── PB12 ── LCD_WR ─── Y(+)
    ──│      ├── PB13 ── LCD_DC ─── X(-)
    ──│      ├── PB14 ── LCD_CS
    ──│      ├── PB15 ── LCD_RST
       ──────

    #define D0 7
    #define D1 6
    #define D2 5
    #define D3 4
    #define D4 3
    #define D5 2
    #define D6 1
    #define D7 0

    #define RD 10
    #define WR 12
    #define DC 13
    #define CS 14
    #define RST 15

    void tft_write_reg(uint8_t eightbit);
    void tft_write_data(uint8_t eightbit);
```
