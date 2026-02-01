

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


boolean AUDIOC = true;

byte MainMenu = 0;    //Переменная для изменения меню
byte MainSelect = 0;  //Переменная для перемещения в главном меню
byte ProgSelect = 0;
byte SettSelect = 0;   //Переменная для перемещения в настройках
byte SettCastom = 0;   //Переменная для переменщения в кастомизации
byte FileSelect = 0;   //Переменная для перемещения в Файлах
byte MAXFILE = 0;      //Переменная макс файлов
String FileName[200];  //Переменная имен файлов
int CursorY = 0;
String FileText;


byte PinS = 0;
int BallX = 110;
int BallY = 110;
int RotateX = 1;
int RotateY = 1;
int Speed = 2;

int PlayerY = 90;
int BotY = 90;

//Создаем объекты экрана и кнопок
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

GButton buttup(BTN_UP);
GButton butty(BTN_Y);
GButton buttdown(BTN_DOWN);

//Функция для перевода String в uint16_t
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
void printCentered(String text, int textSize, int YY) {
  int16_t x1, y1;
  uint16_t w, h;

  tft.setTextSize(textSize);
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Центрирование по горизонтали и вертикали
  int x = (tft.width() - w) / 2;
  //int y = (tft.height() - h) / 2;

  tft.setCursor(x, YY);
  tft.print(text);
}
void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  tft.init(TFT_WIDTH, TFT_HEIGHT);  //Инициализируем экран
  tft.setRotation(2);               //Устанавливаем ориентацию экрана
  tft.setFont(&Bahamas6pt8b);       //Устанавливаем шрифт
  tft.setTextColor(TEXT_COLOR);
  tft.fillScreen(FONE_COLOR);  //Закрашиваем экран

  //Проверка есть ли файл VConfig.json
  if (LittleFS.exists("/VConfig.json")) {
    //Если он есть то просто читаем настройки
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
    //Если нет то создаем новый
    File file = LittleFS.open("/VConfig.json", "w");
    StaticJsonDocument<200> doc;

    doc["Design"]["TextColor"] = TEXTCB;
    doc["Variables"]["AudioCheck"] = AUDIOCB;

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

  //Парсинг команд из Serial
  if (Serial.available() > 0) {
    String key = Serial.readStringUntil('=');

    if (key == "SetConfig") {  //Установка значений
      String val1 = Serial.readStringUntil(',');
      String val2 = Serial.readStringUntil(';');
      if (val2 == "1") {
        AUDIOC = true;
      } else {
        AUDIOC = false;
      }
      TEXT_COLOR = ColorUint(val1);
      tft.setTextColor(TEXT_COLOR);

      //Сохраняем новые настройки
      StaticJsonDocument<256> doc;

      doc["Design"]["TextColor"] = ColorUint(val1);
      doc["Variables"]["AudioCheck"] = AUDIOC;

      File file = LittleFS.open("/VConfig.json", "w");
      serializeJsonPretty(doc, file);
      file.close();
    } else if (key == "VCONFIG") {  //Установка значений
      String val1 = Serial.readStringUntil(';');
      File file1 = LittleFS.open("/VConfig.json", "w");
      file1.print(val1);
      file1.close();

      File file2 = LittleFS.open("/VConfig.json", "r");
      StaticJsonDocument<256> doc;
      DeserializationError err = deserializeJson(doc, file2);  // Парсим JSON
      file2.close();

      String textColor = doc["Design"]["TextColor"].as<String>();
      int audioCheck = doc["Variables"]["AudioCheck"];

      if (audioCheck == 1) {
        AUDIOC = true;
      } else {
        AUDIOC = false;
      }
      TEXT_COLOR = ColorUint(textColor);
      tft.setTextColor(TEXT_COLOR);
    } else if (key == "TXT") {  //Создание txt файлов
      String val1 = Serial.readStringUntil(',');
      String val2 = Serial.readStringUntil(';');

      File file = LittleFS.open("/" + String(val1) + ".txt", "w");
      file.println(val2);
      file.close();
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
    //Условие если нажали на кнопку Y
    if (butty.isClick()) {
      switch (MainSelect) {
        case 0:
          //Переходим на настройки
          MainMenu = 1;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем меню
          settingsmenu();
          break;
        case 1:
          {
            //Переходим в меню настройки
            MainMenu = 4;

            //Очищаем экран
            tft.fillRect(0, 0, 240, 120, FONE_COLOR);

            //Отрисовываем меню без файлов
            filesm();
            tft.setTextSize(2);
            //Начинаем определять сколько файлов
            File root = LittleFS.open("/", "r");
            File file = root.openNextFile();
            //Переменная для отрисовки и сохранения файлов
            int i = 0;
            while (file) {
              //Выбираем позицию
              if (i == 0) {
                tft.setCursor(20, 38 + 18);
              } else if (i > 0) {
                tft.setCursor(20, 38 + 18 * (i + 1));
              }
              //Отрисовываем
              tft.print(file.name());
              //Сохраняем имена файлов в массив
              FileName[i] = file.name();
              //Переходим к следующему файлу
              file = root.openNextFile();
              //Увеличиваем значение
              i++;
            }
            //В конце цикла устонавливаем макс колич файлов
            MAXFILE = i - 1;
            break;
          }
        case 2:
          MainMenu = 6;
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          programm();
          break;
        case 3:
          //Переходим на меню Инфо
          MainMenu = 2;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем меню
          info();
          break;
      }
    }
  }
  //Программы
  if (MainMenu == 6) {
    if (buttdown.isClick() && ProgSelect < 2) {
      ProgSelect++;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (ProgSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * ProgSelect);
        tft.print(">");
      }
    }
    if (buttup.isClick() && ProgSelect > 0) {
      ProgSelect--;
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (ProgSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * ProgSelect);
        tft.print(">");
      }
    }
    if (butty.isClick()) {
      switch (ProgSelect) {
        case 0:
          //Возращаемся в главное меню
          MainMenu = 0;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем главное меню
          mainmenu();
          break;
        case 1:
          MainMenu = 7;
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          pinballmenu();
      }
    }
  }
  if (MainMenu == 7) {
    if (buttup.isClick() & PinS > 0) {
      PinS--;
      if (PinS == 0) {
        tft.fillRect(120 - 40, 40, 15, 80, FONE_COLOR);
        tft.setCursor(120 - 40, 80);
        tft.print(">");
      } else if (PinS == 1) {
        tft.fillRect(120 - 40, 40, 15, 80, FONE_COLOR);
        tft.setCursor(120 - 40, 80 + 18);
        tft.print(">");
      }
    } else if (buttdown.isClick() && PinS < 1) {
      PinS++;
      if (PinS == 0) {
        tft.fillRect(120 - 40, 80, 10, 40, FONE_COLOR);
        tft.setCursor(120 - 40, 80);
        tft.print(">");
      } else if (PinS == 1) {
        tft.fillRect(120 - 40, 80, 10, 40, FONE_COLOR);
        tft.setCursor(120 - 40, 80 + 18);
        tft.print(">");
      }
    }
    if (butty.isClick()) {
      switch (PinS) {
        case 0:
          MainMenu = 8;
          tft.fillScreen(FONE_COLOR);
          game();
          break;
        case 1:
          MainMenu = 6;
          tft.fillScreen(FONE_COLOR);
          programm();
          break;
      }
    }
  }
  if (MainMenu == 8) {
    tft.fillRect(BallX - 10, BallY - 10, 21, 22, FONE_COLOR);
    if (buttup.isClick()) {
      tft.fillRect(0, PlayerY, 10, 60, FONE_COLOR);
      PlayerY -= 20;
      if (PlayerY < 0) {
        PlayerY = 0;
      }
      tft.fillRect(0, PlayerY, 10, 60, TEXT_COLOR);
    } else if (buttdown.isClick()) {
      tft.fillRect(0, PlayerY, 10, 60, FONE_COLOR);
      PlayerY += 20;
      if (PlayerY > 240 - 60) {
        PlayerY = 240 - 60;
      }
      tft.fillRect(0, PlayerY, 10, 60, TEXT_COLOR);
    }
    if (RotateX == 1) {
      BallX += Speed;  //Вправо
    } else if (RotateX == 0) {
      BallX -= Speed;  //Влево
    }
    if (BallX > 150) {
      if (BallY > BotY) {
        tft.fillRect(240 - 10, BotY, 10, 60, FONE_COLOR);
        BotY += 10;
        if (BotY > 240 - 60) {
          BotY = 240 - 60;
        }
      } else if (BallY < BotY) {
        tft.fillRect(240 - 10, BotY, 10, 60, FONE_COLOR);
        BotY -= 10;
        if (BotY < 0) {
          BotY = 0;
        }
      }
    }
    tft.fillRect(240 - 10, BotY, 10, 60, TEXT_COLOR);
    if (BallY > BotY && BallY < BotY + 60) {
      if (BallX > 240 - 23) {
        RotateX = 0;
      }
    }
    if (RotateY == 1) {
      BallY -= Speed;  //Верх
    } else if (RotateY == 0) {
      BallY += Speed;  //Вниз
    }
    if (BallY > 240) {
      RotateY = 1;
    } else if (BallY < 0) {
      RotateY = 0;
    }
    if (BallY > PlayerY && BallY < PlayerY + 60) {
      if (BallX < 22) {
        RotateX = 1;
      }
    }
    if (BallX < 0) {
      MainMenu = 7;
    }

    tft.fillCircle(BallX, BallY, 10, TEXT_COLOR);
  }
  //Файлы
  if (MainMenu == 4) {
    //Перемещение
    if (buttup.isClick() && FileSelect > 0) {
      FileSelect--;
      Serial.println(FileSelect);
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (FileSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * FileSelect);
        tft.print(">");
      }
    }
    if (buttdown.isClick() && FileSelect < MAXFILE + 1) {
      FileSelect++;
      Serial.println(FileSelect);
      tft.fillRect(0, 17, 19, 38 + 18 * 3, FONE_COLOR);
      if (FileSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * FileSelect);
        tft.print(">");
      }
    }
    //Условие при нажатии кнопки Y
    if (butty.isClick()) {
      if (FileSelect == 0) {  //Если выбрали Выход и нажали на кнопку
        //Очищаем экран
        tft.fillScreen(FONE_COLOR);
        //Переходим к главному меню
        MainMenu = 0;
        //Отрисовываем главное меню
        mainmenu();
      } else if (FileSelect > 0) {  //Если выбрали файл и нажали на Y
        //Пишем ОК
        Serial.println("OK");
        //Читаем выбраный файл
        File file = LittleFS.open("/" + FileName[FileSelect - 1], "r");
        FileText = file.readString();
        file.close();

        //Очищаем экран и ставим настройки
        tft.fillScreen(FONE_COLOR);
        tft.setTextSize(1);
        tft.setCursor(0, 7);
        //Отображаем текст
        tft.print(FileText);
        Serial.println(FileText);
        tft.setTextSize(2);
        //Переходи в меню чтения
        MainMenu = 5;
      }
    }
  }
  //Чтение файлов
  if (MainMenu == 5) {
    if (buttup.isClick()) {
      tft.fillRect(0, CursorY - 10, 240, 240 - (CursorY - 10), FONE_COLOR);
      CursorY -= 20;
      tft.setTextSize(1);
      tft.setCursor(0, CursorY);
      tft.print(FileText);
      tft.setTextSize(2);
    } else if (buttdown.isClick()) {
      tft.fillRect(0, CursorY - 10, 240, 240 - (CursorY - 10), FONE_COLOR);
      CursorY += 20;
      tft.setTextSize(1);
      tft.setCursor(0, CursorY);
      tft.print(FileText);
      tft.setTextSize(2);
    }
    //Условие если нажали на кнопку Y
    if (butty.isClick()) {
      //Переходим в меню Файлы
      MainMenu = 4;
      CursorY = 0;
      //Очищаем экран
      tft.fillScreen(FONE_COLOR);
      //Отображаем интерфейс без файлов
      filesm();
      //Отображаем имена
      tft.setTextSize(2);
      File root = LittleFS.open("/", "r");
      File file = root.openNextFile();
      int i = 0;
      while (file) {
        if (i == 0) {
          tft.setCursor(20, 38 + 18);
        } else if (i > 0) {
          tft.setCursor(20, 38 + 18 * (i + 1));
        }
        tft.print(file.name());
        FileName[i] = file.name();
        file = root.openNextFile();
        i++;
      }
      MAXFILE = i - 1;
    }
  }
  //Настройки
  if (MainMenu == 1) {
    //условия при нажатии кнопок
    if (buttup.isClick() && SettSelect > 0) {
      SettSelect--;
      tft.fillRect(0, 17, 19, 38 + 18 * 4, FONE_COLOR);
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
      tft.fillRect(0, 17, 19, 38 + 18 * 4, FONE_COLOR);
      if (SettSelect == 0) {
        tft.setCursor(0, 38);
        tft.print(">");
      } else {
        tft.setCursor(0, 38 + 18 * SettSelect);
        tft.print(">");
      }
    }
    //Условие если нажали на кнопку Y
    if (butty.isClick()) {
      switch (SettSelect) {
        case 0:
          //Возращаемся в главное меню
          MainMenu = 0;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 120, FONE_COLOR);
          //Отрисовываем главное меню
          mainmenu();
          break;
        case 1:
          {
            //Изменяем Значения AUDIO на вкл и выкл


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
            //Перезаписываем VConfig.json
            StaticJsonDocument<256> doc;

            //Ставим новые значения
            doc["Design"]["TextColor"] = TEXT_COLOR;
            doc["Variables"]["AudioCheck"] = AUDIOC;

            //Сохраняем
            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            break;
          }
        case 2:
          //Переходим в меню Кастомизации
          MainMenu = 3;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 150, FONE_COLOR);
          //Отрисовываем меню
          kcastom();

          break;
        case 3:
          {
            //Меню сброса настроек

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

            //Записываем новые значения
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = TEXTCB;
            doc["Variables"]["AudioCheck"] = AUDIOCB;

            //Сохраняем
            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            //Ждем 500мс
            delay(500);
            //Очищаем и выводим картинку
            tft.fillScreen(FONE_COLOR);
            settingsmenu();
            break;
          }
        case 4:
          {
            tft.fillScreen(ST77XX_BLACK);
            LittleFS.format();

            tft.setTextSize(3);
            tft.setCursor((240 - (9 * 20)) / 2, 50);
            tft.print("Требуется");
            tft.setCursor((240 - (12 * 18)) / 2, 100);
            tft.print("Перезагрузка");
            tft.setTextSize(2);
            MainMenu = -1;
            break;
          }
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
    //Условие при нажатии на кнопку Y
    if (butty.isClick()) {
      switch (SettCastom) {
        case 0:
          //Выходим в настройки
          MainMenu = 1;
          //Очищаем экран
          tft.fillRect(0, 0, 240, 150, FONE_COLOR);
          //Отрисовываем меню
          settingsmenu();
          break;
        case 1:
          {
            //Меняем цвет текста
            TEXT_COLOR = ST77XX_WHITE;
            tft.setTextColor(ST77XX_WHITE);
            //Сохраняем новые настройки
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_WHITE";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            tft.fillScreen(FONE_COLOR);
            kcastom();
            break;
          }
        case 2:
          {
            //Меняем цвет текста
            TEXT_COLOR = ST77XX_BLACK;
            tft.setTextColor(ST77XX_BLACK);
            //Сохраняем новые настройки
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_BLACK";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            tft.fillScreen(FONE_COLOR);
            kcastom();
            break;
          }
        case 3:
          {
            //Меняем цвет текста
            TEXT_COLOR = ST77XX_RED;
            tft.setTextColor(ST77XX_RED);
            //Сохраняем новые настройки
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_RED";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            tft.fillScreen(FONE_COLOR);
            kcastom();
            break;
          }
        case 4:
          {
            //Меняем цвет текста
            TEXT_COLOR = ST77XX_GREEN;
            tft.setTextColor(ST77XX_GREEN);
            //Сохраняем новые настройки
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_GREEN";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            tft.fillScreen(FONE_COLOR);
            kcastom();
            break;
          }
        case 5:
          {
            //Меняем цвет текста
            TEXT_COLOR = ST77XX_BLUE;
            tft.setTextColor(ST77XX_BLUE);
            //Сохраняем новые настройки
            StaticJsonDocument<256> doc;

            doc["Design"]["TextColor"] = "ST77XX_BLUE";
            doc["Variables"]["AudioCheck"] = AUDIOC;

            File file = LittleFS.open("/VConfig.json", "w");
            serializeJsonPretty(doc, file);
            file.close();
            tft.fillScreen(FONE_COLOR);
            kcastom();
            break;
          }
      }
    }
  }
  //Инфо
  if (MainMenu == 2) {
    if (butty.isClick()) {
      //При нажатии на кнопку переходим в главное меню
      MainMenu = 0;
      tft.fillScreen(FONE_COLOR);
      mainmenu();
    }
  }
}