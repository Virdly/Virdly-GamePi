//Объект экрана
extern Adafruit_ST7789 tft;

//Переменные
extern uint16_t FONE_COLOR;
extern uint16_t TEXT_COLOR;
extern byte MainSelect;
extern byte SettSelect;
extern bool AUDIOC;

void mainmenu() {
  tft.setTextSize(1);
  tft.setCursor(0, 10);
  tft.print("Главное Меню");
  tft.drawLine(0, 15, TFT_WIDTH, 15, TEXT_COLOR);

  if (AUDIOC == true) {
    tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioON, 8, 7);
  } else {
    tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioOFF, 9, 7);
  }

  tft.setTextSize(2);
  tft.setCursor(20, 38);
  tft.print("Настройки");
  tft.setCursor(20, 38 + 18 * 1);
  tft.print("Файлы");
  tft.setCursor(20, 38 + 18 * 2);
  tft.print("Программы");
  tft.setCursor(20, 38 + 18 * 3);
  tft.print("Инфо");

  if (MainSelect == 0) {
    tft.setCursor(0, 38);
    tft.print(">");
  } else {
    tft.setCursor(0, 38 + 18 * MainSelect);
    tft.print(">");
  }
}
void settingsmenu() {
  tft.setTextSize(1);
  tft.setCursor(0, 10);
  tft.print("Настройки");
  tft.drawLine(0, 15, TFT_WIDTH, 15, TEXT_COLOR);

  if (AUDIOC == true) {
    tft.drawRGBBitmap(20 + 40 + 15, 38 + 7, bitmap_CheckON, 14, 14);
    tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioON, 8, 7);

  } else {
    tft.drawRGBBitmap(20 + 40 + 15, 38 + 7, bitmap_CheckOFF, 14, 14);
    tft.drawRGBBitmap(240 - 8 - 5, 3, bitmap_AudioOFF, 9, 7);
  }

  tft.setTextSize(2);
  tft.setCursor(20, 38);
  tft.print("Выход");
  tft.setCursor(20, 38 + 18 * 1);
  tft.print("Звук");
  tft.setCursor(20, 38 + 18 * 2);
  tft.print("Сброс Настроек");
  tft.setCursor(20, 38 + 18 * 3);
  tft.print("Форматирование");

  if (SettSelect == 0) {
    tft.setCursor(0, 38);
    tft.print(">");
  } else {
    tft.setCursor(0, 38 + 18 * MainSelect);
    tft.print(">");
  }
}
void info() {
  tft.setTextSize(1);
  tft.setCursor(0,7);
  tft.print("Virdly-GamePi это прошивка специально созданая для связки Waveshare-RP2040 PiZero и Waveshare-GamePi13.В прошивке присудствуют базовые функции кастомизации,усановки программ,чтение txt и jpeg файлов,файловый менеджер.  Автор:Rubuhs");
  tft.setTextSize(2);
}