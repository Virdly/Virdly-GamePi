

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
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "GyverButton.h"

#include "Bitmaps.h"   //Битмапы
#include "Bahamas6.h"  //Шрифт
#include "Settings.h"  //Настройки


//Переменные меню и тд

//Битмапы всегда будут цвета FONE_COLOR и TEXT_COLOR даже если изменить параметры VConfig.json
uint16_t FONE_COLOR = ST77XX_BLACK;
uint16_t TEXT_COLOR = ST77XX_WHITE;
boolean AUDIOC = true;

byte MainMenu = 0;
byte MainSelect = 0;
byte SettSelect = 0;
byte SettCastom = 0;

//Создаем объекты экрана и кнопок
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

GButton buttup(BTN_UP);
GButton butty(BTN_Y);
GButton buttdown(BTN_DOWN);

uint16_t ColorUint(String Text) {
  if (Text == "ST77XX_WHITE") {
    return ST77XX_WHITE;
  }
  if (Text == "ST77XX_BLACK") {
    return ST77XX_BLACK;
  }
  if (Text == "ST77XX_RED") {
    return ST77XX_RED;
  }
  if (Text == "ST77XX_GREEN") {
    return ST77XX_GREEN;
  }
  if (Text == "ST77XX_BLUE") {
    return ST77XX_BLUE;
  }
  return ST77XX_WHITE;
};
void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  tft.init(TFT_WIDTH, TFT_HEIGHT);  //Инициализируем экран
  tft.setRotation(2);               //Устанавливаем ориентацию экрана
  tft.setFont(&Bahamas6pt8b);       //Устанавливаем шрифт
  tft.setTextColor(TEXT_COLOR);
  tft.fillScreen(FONE_COLOR);  //Закрашиваем экран
  if (LittleFS.exists("/VConfig.json")) {
    File file = LittleFS.open("/VConfig.json", "r");
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, file);  // Парсим JSON
    file.close();

    String textColor = doc["Design"]["TextColor"].as<String>();
    int audioCheck = doc["Variables"]["AudioCheck"];

    TEXT_COLOR = ColorUint(textColor);
    tft.setTextColor(TEXT_COLOR);

    if (audioCheck == 1) {
      AUDIOC = true;
    } else {
      AUDIOC = false;
    }

  } else {
    File file = LittleFS.open("/VConfig.json", "w");
    StaticJsonDocument<200> doc;

    doc["Design"]["TextColor"] = "ST77XX_WHITE";
    doc["Variables"]["AudioCheck"] = 1;

    serializeJson(doc, file);
    file.close();
  }
  mainmenu();  //Отображаем главное меню
}
void loop() {
  //Опрашиваем кнопки
  buttup.tick();
  butty.tick();
  buttdown.tick();
  if (Serial.available() > 0) {
    String key = Serial.readStringUntil('=');
    String val = Serial.readStringUntil(';');
    if (key == "VCONFIG") {
      File file = LittleFS.open("/VConfig.json", "w");
      file.println(val);
      file.close();

      File file1 = LittleFS.open("/VConfig.json", "r");
      StaticJsonDocument<256> doc1;
      DeserializationError err = deserializeJson(doc1, file1);  // Парсим JSON
      file.close();

      String textColor = doc1["Design"]["TextColor"].as<String>();
      int audioCheck = doc1["Variables"]["AudioCheck"];

      if (audioCheck == 1) {
        AUDIOC = true;
      } else {
        AUDIOC = false;
      }

      TEXT_COLOR = ColorUint(textColor);
      tft.setTextColor(TEXT_COLOR);
    }
    if (key = "SetConfig") {
      String val1 = Serial.readStringUntil(',');
      String val2 = Serial.readStringUntil(';');

      StaticJsonDocument<256> doc;

      doc["Design"]["TextColor"] = val1;
      doc["Variables"]["AudioCheck"] = val2;

      File file = LittleFS.open("/VConfig.json", "w");
      serializeJsonPretty(doc, file);
      file.close();

      TEXT_COLOR = ColorUint(val1);
      tft.setTextColor(TEXT_COLOR);

      if (val2 == "1") {
        AUDIOC = 1;
      } else {
        AUDIOC = 0;
      }
    }
  }

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
          //Изменяем переменную Меню
          MainMenu = 1;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем меню
          settingsmenu();
          break;
        case 1:
          //В будущем быдут добавлены
          break;
        case 2:
          //В будущем быдут добавлены
          break;
        case 3:
          //Изменяем переменную Меню
          MainMenu = 2;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем меню
          info();
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
    if (buttdown.isClick() && SettSelect < 4) {
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
          //Изменяем переменную Меню
          MainMenu = 0;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем главное меню
          mainmenu();
          break;
        case 1:
          {
            //Очищаем старые места битмапов
            tft.fillRect(75, 38 + 7, 14, 14, FONE_COLOR);
            tft.fillRect(240 - 8 - 5, 3, 10, 7, FONE_COLOR);
            //Меняем значение вкл/выкл
            AUDIOC = !AUDIOC;

            //Рисуем обновленные битмапы
            if (AUDIOC == true) {
              tft.drawRGBBitmap(20 + 40 + 15, 38 + 7, bitmap_CheckON, 14, 14);
              tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioON, 8, 7);
            } else {
              tft.drawRGBBitmap(20 + 40 + 15, 38 + 7, bitmap_CheckOFF, 14, 14);
              tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioOFF, 9, 7);
            }
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = TEXT_COLOR;
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 2:
          //Изменяем переменную Меню
          MainMenu = 3;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 150, FONE_COLOR);
          //Отрисовываем главное меню
          kcastom();

          break;
        case 3:
          {
            //Меняем значения на стандартные
            FONE_COLOR = FONECB;
            TEXT_COLOR = TEXTCB;
            AUDIOC = AUDIOCB;
            //Устонавливаем цвет текста
            tft.setTextColor(TEXT_COLOR);
            //Очищаем экран
            tft.fillScreen(FONE_COLOR);
            //Выводим текст на экран
            tft.setTextSize(3);
            tft.setCursor((240 - (2 * 25)) / 2, 100);
            tft.print("ОК");
            tft.setTextSize(2);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_WHITE";
            doc["Variables"]["AudioCheck"] = 1;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            //Ждем 500мс
            delay(500);
            tft.fillScreen(FONE_COLOR);
            settingsmenu();

            break;
          }
        case 4:
          break;
      }
    }
  }
  //Кастомизация
  if (MainMenu == 3) {
    //условия при нажатии кнопок
    if (buttup.isClick() && SettCastom > 0) {
      SettCastom--;
      tft.fillRect(0, 17, 19, 38 + 18 * 5, FONE_COLOR);
      if (SettCastom == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * SettCastom);
        tft.print(">");
      }
    }
    if (buttdown.isClick() && SettCastom < 5) {
      SettCastom++;
      tft.fillRect(0, 17, 19, 38 + 18 * 5, FONE_COLOR);
      if (SettCastom == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * SettCastom);
        tft.print(">");
      }
    }
    if (butty.isClick()) {
      switch (SettCastom) {
        case 0:
          MainMenu = 1;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 150, FONE_COLOR);
          //Отрисовываем главное меню
          settingsmenu();
          break;
        case 1:
          {
            TEXT_COLOR = ST77XX_WHITE;
            tft.setTextColor(ST77XX_WHITE);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_WHITE";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 2:
          {
            TEXT_COLOR = ST77XX_BLACK;
            tft.setTextColor(ST77XX_BLACK);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_BLACK";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 3:
          {
            TEXT_COLOR = ST77XX_RED;
            tft.setTextColor(ST77XX_RED);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_RED";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 4:
          {
            TEXT_COLOR = ST77XX_GREEN;
            tft.setTextColor(ST77XX_GREEN);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_GREEN";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 5:
          {
            TEXT_COLOR = ST77XX_BLUE;
            tft.setTextColor(ST77XX_BLUE);
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_BLUE";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
      }
    }
  }
  //Инфо
  if (MainMenu == 2) {
    if (butty.isClick()) {
      MainMenu = 0;
      tft.fillScreen(FONE_COLOR);
      mainmenu();
    }
  }
}