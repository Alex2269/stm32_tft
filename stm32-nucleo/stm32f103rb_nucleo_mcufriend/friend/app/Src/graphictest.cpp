#include "graphictest.hpp"

uint16_t g_identifier;

MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
    /*Serial.begin(9600);
    uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) HAL_Delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    //    tft.reset();                 //hardware reset*/
    uint16_t ID = tft.readID(); //
  //  Serial.print("ID = 0x");
  //  Serial.println(ID, HEX);
  //  if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID*/
    tft.begin(ID);
}

//#if defined(MCUFRIEND_KBV_HPP_)
uint16_t scrollbuf[320];    // my biggest screen is 320x480
#define READGRAM(x, y, buf, w, h)  tft.readGRAM(x, y, buf, w, h)
/*#else
// Adafruit can read a block by one pixel at a time
int16_t  READGRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h)
{
    uint16_t *p;
    for (int row = 0; row < h; row++) {
        p = block + row * w;
        for (int col = 0; col < w; col++) {
            *p++ = tft.readPixel(x + col, y + row);
        }
    }
}
#endif*/

void windowScroll(int16_t x, int16_t y, int16_t wid, int16_t ht, int16_t dx, int16_t dy, uint16_t *buf)
{
    if (dx) for (int16_t row = 0; row < ht; row++) {
            READGRAM(x, y + row, buf, wid, 1);
            tft.setAddrWindow(x, y + row, x + wid - 1, y + row);
            tft.pushColors(buf + dx, wid - dx, 1);
            tft.pushColors(buf + 0, dx, 0);
        }
    if (dy) for (int16_t col = 0; col < wid; col++) {
            READGRAM(x + col, y, buf, 1, ht);
            tft.setAddrWindow(x + col, y, x + col, y + ht - 1);
            tft.pushColors(buf + dy, ht - dy, 1);
            tft.pushColors(buf + 0, dy, 0);
        }
}

void printmsg(int row, const char *msg)
{
    tft.setTextColor(YELLOW, BLACK);
    tft.setCursor(0, row);
    tft.println(msg);
}

void loop(void) {
    uint8_t aspect;
    uint16_t pixel;
    const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
    
    const char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
    uint16_t colormask[] = { 0x001F, 0x07E0, 0xF800, 0xFFFF };
    uint16_t dx, rgb, n, wid, ht, msglin;
    tft.setRotation(0);
    runtests();
    HAL_Delay(2000);
    
    if (tft.height() > 64) {
        for (uint8_t cnt = 0; cnt < 4; cnt++) {
            aspect = (cnt + 0) & 3;
            tft.setRotation(aspect);
            wid = tft.width();
            ht = tft.height();
            msglin = (ht > 160) ? 200 : 112;
            testText();
            dx = wid / 32;
            for (n = 0; n < 32; n++) {
                rgb = n * 8;
                rgb = tft.color565(rgb, rgb, rgb);
                tft.fillRect(n * dx, 48, dx, 63, rgb & colormask[aspect]);
            }
            tft.drawRect(0, 48 + 63, wid, 1, WHITE);
            tft.setTextSize(2);
            tft.setTextColor(colormask[aspect], BLACK);
            tft.setCursor(0, 72);
            tft.print(colorname[aspect]);
            tft.setTextColor(WHITE);
            tft.println(" COLOR GRADES");
            tft.setTextColor(WHITE, BLACK);
            printmsg(184, aspectname[aspect]);
            HAL_Delay(1000);
            tft.drawPixel(0, 0, YELLOW);
            pixel = tft.readPixel(0, 0);
            tft.setTextSize((ht > 160) ? 2 : 1); //for messages


            extern const uint8_t penguin[];
            tft.setAddrWindow(wid - 40 - 40, 20 + 0, wid - 1 - 40, 20 + 39);
            tft.pushColors(penguin, 1600, 1);


            tft.setAddrWindow(0, 0, wid - 1, ht - 1);
            if (aspect & 1) tft.drawRect(wid - 1, 0, 1, ht, WHITE);
            else tft.drawRect(0, ht - 1, wid, 1, WHITE);
            printmsg(msglin, "VERTICAL SCROLL UP");
            uint16_t maxscroll;
            if (tft.getRotation() & 1) maxscroll = wid;
            else maxscroll = ht;
            for (uint16_t i = 1; i <= maxscroll; i++) {
                tft.vertScroll(0, maxscroll, i);
                HAL_Delay(10);
            }
            HAL_Delay(1000);
			printmsg(msglin, "VERTICAL SCROLL DN");
            for (uint16_t i = 1; i <= maxscroll; i++) {
                tft.vertScroll(0, maxscroll, 0 - (int16_t)i);
                HAL_Delay(10);
            }
			tft.vertScroll(0, maxscroll, 0);
            printmsg(msglin, "SCROLL DISABLED   ");

            HAL_Delay(1000);
            if ((aspect & 1) == 0) { //Portrait
                tft.setTextColor(BLUE, BLACK);
                printmsg(msglin, "ONLY THE COLOR BAND");
                for (uint16_t i = 1; i <= 64; i++) {
                    tft.vertScroll(48, 64, i);
                    HAL_Delay(20);
                }
                HAL_Delay(1000);
            }

            tft.setTextColor(YELLOW, BLACK);
            if (pixel == YELLOW) {
                printmsg(msglin, "SOFTWARE SCROLL    ");
                
         // plain horizontal scroll of block
                n = (wid > 320) ? 320 : wid;
                for (int16_t i = n, dx = 4, dy = 0; i > 0; i -= dx) {
                    windowScroll(0, 200, n, 16, dx, dy, scrollbuf);
                }

            }
            else if (pixel == CYAN)
                tft.println("readPixel() reads as BGR");
            else if ((pixel & 0xF8F8) == 0xF8F8)
                tft.println("readPixel() should be 24-bit");
            else {
                tft.print("readPixel() reads 0x");
                tft.println(pixel, HEX);
            }
            HAL_Delay(5000);
        }
    }
    printmsg(msglin, "INVERT DISPLAY ");
    tft.invertDisplay(true);
    HAL_Delay(2000);
    tft.invertDisplay(false);
}

typedef struct {
    PGM_P msg;
    uint32_t ms;
} TEST;
TEST result[12];

#define RUNTEST(n, str, test) { result[n].msg = PSTR(str); result[n].ms = test; HAL_Delay(500); }

void runtests(void)
{
    uint8_t i, len = 24;
    uint32_t total;
    RUNTEST(0, "FillScreen               ", testFillScreen());
    RUNTEST(1, "Text                     ", testText());
    RUNTEST(2, "Lines                    ", testLines(CYAN));
    RUNTEST(3, "Horiz/Vert Lines         ", testFastLines(RED, BLUE));
    RUNTEST(4, "Rectangles (outline)     ", testRects(GREEN));
    RUNTEST(5, "Rectangles (filled)      ", testFilledRects(YELLOW, MAGENTA));
    RUNTEST(6, "Circles (filled)         ", testFilledCircles(10, MAGENTA));
    RUNTEST(7, "Circles (outline)        ", testCircles(10, WHITE));
    RUNTEST(8, "Triangles (outline)      ", testTriangles());
    RUNTEST(9, "Triangles (filled)       ", testFilledTriangles());
    RUNTEST(10, "Rounded rects (outline)  ", testRoundRects());
    RUNTEST(11, "Rounded rects (filled)   ", testFilledRoundRects());

    tft.fillScreen(BLACK);
    tft.setTextColor(GREEN);
    tft.setCursor(0, 0);
    uint16_t wid = tft.width();
    if (wid > 176) {
        tft.setTextSize(2);
        
#if defined(MCUFRIEND_KBV_HPP_)
        tft.print("MCUFRIEND ");
#if MCUFRIEND_KBV_HPP_ != 0
        tft.print(0.01 * MCUFRIEND_KBV_HPP_, 2);
#else
        tft.print("for");
#endif
        tft.println(" UNO");
#else
        tft.println("Adafruit-Style Tests");
#endif

    } else len = wid / 6 - 8;
    tft.setTextSize(1.5);
    total = 0;
    for (i = 0; i < 12; i++) {
        PGM_P str = result[i].msg;
        char c;
        if (len > 24) {
            if (i < 10) tft.print(" ");
            tft.print(i);
            tft.print(": ");
        }
        uint8_t cnt = len;
        while ((c = pgm_read_byte(str++)) && cnt--) tft.print(c);
        tft.print(" ");
        tft.println(result[i].ms);
        total += result[i].ms;
    }
    tft.setTextSize(2);
    tft.print("Total:");
    tft.print(0.000001 * total);
    tft.println("sec");
    g_identifier = tft.readID();
    tft.print("ID: 0x");
    tft.println(tft.readID(), HEX);
//    tft.print("Reg(00):0x");
//    tft.println(tft.readReg(0x00), HEX);
    tft.print("F_CPU:");
    tft.print(0.000001 * F_CPU);
#if defined(__OPTIMIZE_SIZE__)
    tft.println("MHz -Os");
#else
    tft.println("MHz");
#endif

    HAL_Delay(10000);
}

// Standard Adafruit tests.  will adjust to screen size

unsigned long testFillScreen() {
    unsigned long start = micros();
    tft.fillScreen(BLACK);
    HAL_Delay(1000);
    tft.fillScreen(RED);
    HAL_Delay(1000);
    tft.fillScreen(GREEN);
    HAL_Delay(1000);
    tft.fillScreen(BLUE);
    HAL_Delay(1000);
    tft.fillScreen(BLACK);
    HAL_Delay(1000);
    return micros() - start;
}

unsigned long testText() {
    unsigned long start;
    tft.fillScreen(BLACK);
    start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(YELLOW); tft.setTextSize(2);
    tft.println(123.45);
    tft.setTextColor(RED);    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();
    tft.println();
    tft.println();
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println("Faust");
    tft.setTextSize(1);
    tft.println("Da stehe ich nun,ich armer Tor");
    tft.setTextSize(1);
    tft.println("Und bin so klug als wie zuvor;");
    tft.println("Hei�e Magister, hei�e Doctor gar,");
    tft.println("Und ziehe schon an die zehn Jahr");
    tft.println("Herauf, herab und quer und krumm");
    tft.println("Meine Schueler an der Nase herum");
    HAL_Delay(1000);
    return micros() - start;
}

unsigned long testLines(uint16_t color) {
    unsigned long start, t;
    int           x1, y1, x2, y2,
                  w = tft.width(),
                  h = tft.height();

    tft.fillScreen(BLACK);

    x1 = y1 = 0;
    y2    = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t     = micros() - start; // fillScreen doesn't count against timing

    tft.fillScreen(BLACK);

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    tft.fillScreen(BLACK);

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    tft.fillScreen(BLACK);

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
    unsigned long start;
    int           x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(BLACK);
    start = micros();
    for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color) {
    unsigned long start;
    int           n, i, i2,
                  cx = tft.width()  / 2,
                  cy = tft.height() / 2;

    tft.fillScreen(BLACK);
    n     = min(tft.width(), tft.height());
    start = micros();
    for (i = 2; i < n; i += 6) {
        i2 = i / 2;
        tft.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
    unsigned long start, t = 0;
    int           n, i, i2,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n = min(tft.width(), tft.height());
    for (i = n; i > 0; i -= 6) {
        i2    = i / 2;
        start = micros();
        tft.fillRect(cx - i2, cy - i2, i, i, color1);
        t    += micros() - start;
        // Outlines are not included in timing results
        tft.drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(BLACK);
    start = micros();
    for (x = radius; x < w; x += r2) {
        for (y = radius; y < h; y += r2) {
            tft.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
    unsigned long start;
    int           x, y, r2 = radius * 2,
                        w = tft.width()  + radius,
                        h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2) {
        for (y = 0; y < h; y += r2) {
            tft.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles() {
    unsigned long start;
    int           n, i, cx = tft.width()  / 2 - 1,
                        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n     = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5) {
        tft.drawTriangle(
            cx    , cy - i, // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            tft.color565(0, 0, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles() {
    unsigned long start, t = 0;
    int           i, cx = tft.width()  / 2 - 1,
                     cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5) {
        start = micros();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                         tft.color565(0, i, i));
        t += micros() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                         tft.color565(i, i, 0));
    }

    return t;
}

unsigned long testRoundRects() {
    unsigned long start;
    int           w, i, i2, red, step,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    w     = min(tft.width(), tft.height());
    start = micros();
    red = 0;
    step = (256 * 6) / w;
    for (i = 0; i < w; i += 6) {
        i2 = i / 2;
        red += step;
        tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(red, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects() {
    unsigned long start;
    int           i, i2, green, step,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    green = 256;
    step = (256 * 6) / min(tft.width(), tft.height());
    for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
        i2 = i / 2;
        green -= step;
        tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, green, 0));
    }

    return micros() - start;
}
