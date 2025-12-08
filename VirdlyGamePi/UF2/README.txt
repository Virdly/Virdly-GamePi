Стоят стандартные настройки:

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

//Переменные меню и тд
uint16_t FONE_COLOR = ST77XX_BLACK;
uint16_t TEXT_COLOR = ST77XX_WHITE;
boolean AUDIOC = true;

byte MainMenu = 0;
byte MainSelect = 0;
byte SettSelect = 0;