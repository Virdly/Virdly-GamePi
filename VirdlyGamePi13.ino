// Код ещё не полностью готов, в скором времени буду дорабатывать


/*
Параметры платы


Плaта: "Waveshare RP2040 PiZero"

Debug Port: "Disabled"

C++ Exceptions: "Disabled"

Flash Size: "16MB (Sketch: 11MB, FS: 5MB)"

CPU Speed: "200 MHz"
IP/Bluetooth Stack: "IPv4 Only"
Optimize: "Small (-Os) (standard)"
Operating System: "None"

Profiling: "Disabled"

RTTI: "Disabled"

Stack Protector: "Disabled"

Upload Method: "Default (UF2)"
USB Stack: "Pico SDK"

Если у вас еще не установлены платы Rasbery Pi Pico то добавте эту ссылку в "Параметры\Дополнительные ссылки для Менеджера плат"
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

*/



//Загрузка библиотек
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "GyverButton.h"
#include "Bahamas6.h"
#include "Settings.h"
#include "Bitmaps.h"

//Переменные меню и тд
uint16_t FONE_COLOR = ST77XX_BLACK;
uint16_t TEXT_COLOR = ST77XX_WHITE;
boolean AUDIOC = true;

byte MainMenu = 0;
byte MainSelect = 0;
byte SettSelect = 0;

//Создаем объекты экрана и кнопок
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

GButton buttup(BTN_UP);
GButton butty(BTN_Y);
GButton buttdown(BTN_DOWN);


void setup() {
  tft.init(TFT_WIDTH, TFT_HEIGHT); //Инициализируем экран
  tft.setRotation(2); //Устанавливаем ориентацию экрана
  tft.fillScreen(FONE_COLOR); //Закрашиваем экран
  tft.setFont(&Bahamas6pt8b); //Устанавливаем шрифт
  tft.setTextColor(TEXT_COLOR); //Устанавливаем цвет текста
  mainmenu(); //Отображаем главное меню
}

void loop() {
  //Опрашиваем кнопки
  buttup.tick();
  butty.tick();
  buttdown.tick();

  //Главное меню
  if (MainMenu == 0) {
    //условия при нажатии кнопок
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
  //Настройки
  if (MainMenu == 1) {
    //условия при нажатии кнопок
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
