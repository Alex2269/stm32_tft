#include <GxTFT.h> // Hardware-specific library

#define TFT_Class GxTFT

#include<GxIO/STM32DUINO/GxIO_STM32F4_FSMC/GxIO_STM32F4_FSMC.h>
#include <GxCTRL/GxCTRL_ILI9486/GxCTRL_ILI9486.h> // 320x480 e.g. 3.5inch RPI Display
#include "myTFTs/my_3.5_TFT_480x320_ILI9486_STM32F407ZGM4_FSMC.h"

#include "GxReadRegisters.h"

static const int TIME_BUTTON = PB2;
static const int TRIGGER_BUTTON = PA0;
static const int FREEZE_BUTTON = PC1;
static const int TEST_SIGNAL = PA8;
static const int CHANNEL_1 = PA3;

#define BLACK        0x0000 //   0,   0,   0
#define NAVY         0x000F //   0,   0, 128
#define DARKGREEN    0x03E0 //   0, 128,   0
#define DARKCYAN     0x03EF //   0, 128, 128
#define MAROON       0x7800 // 128,   0,   0
#define PURPLE       0x780F // 128,   0, 128
#define OLIVE        0x7BE0 // 128, 128,   0
#define LIGHTGREY    0xC618 // 192, 192, 192
#define DARKGREY     0x7BEF // 128, 128, 128
#define BLUE         0x001F //   0,   0, 255
#define GREEN        0x07E0 //   0, 255,   0
#define CYAN         0x07FF //   0, 255, 255
#define RED          0xF800 // 255,   0,   0
#define MAGENTA      0xF81F // 255,   0, 255
#define YELLOW       0xFFE0 // 255, 255,   0
#define WHITE        0xFFFF // 255, 255, 255
#define ORANGE       0xFD20 // 255, 165,   0
#define GREENYELLOW  0xAFE5 // 173, 255,  47
#define PINK         0xF81F

static const int BACKGROUND_COLOR = BLACK;
static const int DIV_LINE_COLOR = DARKGREY;
static const int CH1_SIGNAL_COLOR = ORANGE;

static const int TOP_BAR = 20;
static const int ADC_RESOLUTION = 4096; // units
static const int EFFECTIVE_VERTICAL_RESOLUTION = 280; // pixels
static const int SCREEN_HORIZONTAL_RESOLUTION = 480; // pixels
static const int SCREEN_VERTICAL_RESOLUTION = 320; // pixels
static const int DIVISION_SIZE = 30; // pixels
static const float ADC_SCREEN_FACTOR = (float)EFFECTIVE_VERTICAL_RESOLUTION / (float)ADC_RESOLUTION;
static const int BUFFER_SIZE = 1024; // bytes
static const byte TRIGGER_THRESOLD = 127; // units
int clear_count;


int data[BUFFER_SIZE], bk[BUFFER_SIZE];
int i, j, h = 1, h2 = -1, counter, startIndex, xOffset = 10, yOffset = 20, temp, old;
byte state = 0, k, dt = 0, dti = 5, trigger = 1, freeze = 0;
bool bPress[3], bChannelChange = true, bScreenChange = true;
const int DT_LIST[] = {0, 8, 12, 14, 15, 16, 17, 18, 20, 24, 32};
const int DT_DIV[] = {12, 24, 49, 98, 196, 392, 783, 2, 3, 7, 13};
volatile int tmpdata;

bool wasPressed(int pin, int index) {
  //
  if (HIGH == digitalRead(pin)) {
    // isn't pressed
    if (bPress[index]) {
      // but was before
      bPress[index] = false;
    }
    return false;
  }
  // is pressed
  if (!bPress[index]) {
    // and wasn't before
    bPress[index] = true;
    return true;
  }
  // but was before
  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(CHANNEL_1, INPUT_ANALOG);
  // pinMode (CHANNEL_1, INPUT);
  #define dsp_rst PC5
  pinMode(PC5, OUTPUT);
  digitalWrite(dsp_rst, 0);
  delay(50);
  digitalWrite(dsp_rst, 1);
  delay(50);
  tft.init();

  tft.setRotation(1);
  bPress[0] = false;
  bPress[1] = false;
  bPress[2] = false;
}

void loop() {
  //
  if (state == 0) {
    // 
    tft.fillScreen(BACKGROUND_COLOR);
    tft.setCursor(SCREEN_HORIZONTAL_RESOLUTION/2-40, SCREEN_VERTICAL_RESOLUTION/2-20);
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.println("Hello");
    analogWrite(TEST_SIGNAL, 127);
    delay(1500);
    tft.fillScreen(BACKGROUND_COLOR);
    state = 1;
  }
  if (state == 1) {
    // init
    counter = 0;
    startIndex = -1;
    old = -1;
    i = 0;
    state = 2;
  }
  if (state == 2) {
    // buttons
    if (wasPressed(TIME_BUTTON, 0)) {
      // toggling the time division modes
      dti ++;
      if(dti>10)dti=0;
      dti = dti % sizeof(DT_LIST);
      dt = DT_LIST[dti];
      h = (dti < 5) ? -2 * (dt - 16) : 1;
      bChannelChange = true;
    }
    if (wasPressed(TRIGGER_BUTTON, 1)) {
      // toggling the trigger mode
      trigger ++;
      trigger = trigger % 3;
      bChannelChange = true;
    }
    if (wasPressed(FREEZE_BUTTON, 2)) {
      // toggling the freeze screen
      freeze = 0; // ^= 0x01;
      bChannelChange = true;
    }
    if (freeze) {
      // frozen screen
      state = 5;
    } else {
      // live screen
      state = 3;
    }
  }
  if (state == 3) {
    // acquisition
    for (;counter < BUFFER_SIZE;) {
      //
      for (k = 0; k < dt - 16; k ++) {
        // 
        tmpdata = (byte) ((float)analogRead(CHANNEL_1) * ADC_SCREEN_FACTOR) + 1;
      }
      data[i] = (byte) ((float)analogRead(CHANNEL_1) * ADC_SCREEN_FACTOR) + 1;
      if (startIndex < 0) {
        // trigger condition not met
        if (trigger == 1) {
          // raising front trigger
          if ((old > -1) && (old < TRIGGER_THRESOLD) && (data[i] >= TRIGGER_THRESOLD)) {
            // 
            startIndex = i;
          }        
        } else if (trigger == 2) {
          // descending front trigger
          if ((old > -1) && (old > TRIGGER_THRESOLD) && (data[i] <= TRIGGER_THRESOLD)) {
            // 
            startIndex = i;
          }
        } else {
          // no trigger
          startIndex = i;
        }
      } else {
        // trigger condition met
        counter ++;
      }
      i ++;
      if (i >= BUFFER_SIZE) i = 0;
      old = data[i];
    }
    state = 4;
  }
  if (state == 4) {
    // 
    if (bScreenChange) {
      // 
      bScreenChange = false;
      tft.fillScreen(BACKGROUND_COLOR);
      bChannelChange = true;
    } else {
      // clear previous wave
      clear_count++;
      if(clear_count>300) // periodic clear heap
      {
        tft.fillRect(0, TOP_BAR, SCREEN_HORIZONTAL_RESOLUTION, SCREEN_VERTICAL_RESOLUTION, BACKGROUND_COLOR);
        clear_count=0;
        
      }
      if (h2 > 0) {
        // 
        for (i = 0, j = 0; j < SCREEN_HORIZONTAL_RESOLUTION; i ++, j += h2) {
          // 
          tft.drawLine(j,bk[i], j+h2, bk[i + 1], BACKGROUND_COLOR);
        }
      }
    }
    // re-draw the divisions horizontal
    for (i = 0; i < SCREEN_HORIZONTAL_RESOLUTION; i += DIVISION_SIZE/8) {
      // 
      for (j = TOP_BAR; j < SCREEN_VERTICAL_RESOLUTION; j += ((i == SCREEN_HORIZONTAL_RESOLUTION/2) ? 4 : DIVISION_SIZE)) {
        // 
        tft.drawLine(i - 1, j, i + 1, j, DIV_LINE_COLOR);
      }
    }
    // re-draw the divisions vertical
    for (i = 0; i < SCREEN_VERTICAL_RESOLUTION; i += DIVISION_SIZE/8) {
      // 
      for (j = 0; j < SCREEN_HORIZONTAL_RESOLUTION; j += ((i == SCREEN_VERTICAL_RESOLUTION/2) ? 4 : DIVISION_SIZE)) {
        //
        tft.drawLine(j, TOP_BAR+i - 1, j, TOP_BAR+i + 1, DIV_LINE_COLOR);
      }
    }
    // draw current wave
    for (i = 0, j = 0; j < SCREEN_HORIZONTAL_RESOLUTION; i ++, j += h) {
      // 
      tft.drawLine(
        j, 
        SCREEN_VERTICAL_RESOLUTION - (yOffset + data[(i + startIndex + BUFFER_SIZE - xOffset) % BUFFER_SIZE]), 
        j + h, 
        SCREEN_VERTICAL_RESOLUTION - (yOffset + data[(i + 1 + startIndex + BUFFER_SIZE - xOffset) % BUFFER_SIZE]), 
        CH1_SIGNAL_COLOR);
      bk[i] = SCREEN_VERTICAL_RESOLUTION - (yOffset + data[(i + startIndex + BUFFER_SIZE - xOffset) % BUFFER_SIZE]);
    }
    h2 = h;
    state = 5;
  }
  if (state == 5) {
    // 
    if (bChannelChange) {
      // channel change
      bChannelChange = false;
      tft.fillRect(0, 0, SCREEN_HORIZONTAL_RESOLUTION, TOP_BAR, DARKGREY);
      tft.setCursor(10, 3);
      tft.setTextColor(BLUE);
      tft.setTextSize(2);
      String s = "CH1 ";
      s += 660;
      s += "mV ";
      s += DT_DIV[dti];
      s += ((dti < 7) ? "us" : "ms");
      tft.print(s);
      if (trigger == 1) {
        // raising front trigger
        tft.setCursor(310, 3);
        tft.setTextColor(MAGENTA);
        tft.setTextSize(2);
        tft.print("Hi");
      } else if (trigger == 2) {
        // descending front trigger
        tft.setCursor(310, 3);
        tft.setTextColor(MAGENTA);
        tft.setTextSize(2);
        tft.print("Lo");
      } else {
        // no trigger
        tft.setCursor(310, 3);
        tft.setTextColor(MAGENTA);
        tft.setTextSize(2);
        tft.print("X");
      }
      if (freeze) {
        // 
        tft.setCursor(360, 3);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.print("Freeze");
      } else {
        tft.setCursor(360, 3);
        tft.setTextColor(BLUE);
        tft.setTextSize(2);
        tft.print("Running");
      }
      tft.setCursor(440, 3);
      tft.setTextColor(BLACK);
      tft.setTextSize(1);
      tft.print("scope");
    }
  }
  delay(100);
  state = 1;
}
