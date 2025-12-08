//Loading library
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "GyverButton.h"
#include "Bahamas6.h"
#include "Settings.h"
#include "Bitmaps.h"

//Variables in the main menu, etc.
uint16_t FONE_COLOR = ST77XX_BLACK;
uint16_t TEXT_COLOR = ST77XX_WHITE;
boolean AUDIOC = true;

byte MainMenu = 0;
byte MainSelect = 0;
byte SettSelect = 0;

//Creating a screen and buttons object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

GButton buttup(BTN_UP);
GButton butty(BTN_Y);
GButton buttdown(BTN_DOWN);


void setup() {
  Serial.begin(115200);
  tft.init(240, 240);
  tft.setRotation(2);
  tft.fillScreen(FONE_COLOR);
  tft.setFont(&Bahamas6pt8b);
  tft.setTextColor(TEXT_COLOR);
  mainmenu();
}

void loop() {
  buttup.tick();
  butty.tick();
  buttdown.tick();

  //MainMenu
  if (MainMenu == 0) {
    if (buttup.isClick() && MainSelect > 0) {
      MainSelect--;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (MainSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * MainSelect);
        tft.print(">");
      }
    }
    if (buttdown.isClick() && MainSelect < 3) {
      MainSelect++;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (MainSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * MainSelect);
        tft.print(">");
      }
    }
    if (butty.isClick()) {
      switch (MainSelect) {
        case 0:
          MainMenu = 1;
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          settingsmenu();
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
      }
    }
  }
  //Settings
  if (MainMenu == 1) {
    if (buttup.isClick() && SettSelect > 0) {
      SettSelect--;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (SettSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * SettSelect);
        tft.print(">");
      }
    }
    if (buttdown.isClick() && SettSelect < 3) {
      SettSelect++;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (SettSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * SettSelect);
        tft.print(">");
      }
    }
    if (butty.isClick()) {
      switch (SettSelect) {
        case 0:
          MainMenu = 0;
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          mainmenu();
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
      }
    }
  }
}
