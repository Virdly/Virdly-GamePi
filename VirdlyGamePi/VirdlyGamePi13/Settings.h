// Ширина и высота экрана
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// Пины экрана
#define TFT_CS   8
#define TFT_RST  27
#define TFT_DC   25
#define TFT_SCLK 10
#define TFT_MOSI 11

// Пины кнопок
#define BTN_Y      9
#define BTN_X      5
#define BTN_A      21
#define BTN_B      20
#define BTN_START  26
#define BTN_SELECT 19
#define BTN_L      23
#define BTN_R      4
#define BTN_LEFT   16
#define BTN_UP     15
#define BTN_RIGHT  13
#define BTN_DOWN    6

// Пин динамика
#define BUZ 18

//Битмапы всегда будут цвета FONE_COLOR и TEXT_COLOR даже если изменить параметры VConfig.json
uint16_t FONE_COLOR = ST77XX_BLACK;
uint16_t TEXT_COLOR = ST77XX_WHITE;

//Стандартные настройки при сбросе
#define AUDIOCB 1
uint16_t FONECB = ST77XX_BLACK;
uint16_t TEXTCB = ST77XX_WHITE;