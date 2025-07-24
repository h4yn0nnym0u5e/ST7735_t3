#include <ST7735_t3_font_Arial.h>
#include <ST7735_t3_font_OpenSans.h>

#define ILI9341_CS 32
#define ST7789_CS  31
#define GC9A01_CS  30
#define ST7796_CS  29
#define ST7735_CS  28 

#define GBL_BLK 33
#define GBL_RST 34

#define TFT_DC    9
#define TFT_SCK  13
#define TFT_MISO 12
#define TFT_MOSI 11


#define TFT_BLK   GBL_BLK
#define TFT_RST   255 // only use for one screen

#define TFT_ROTATE 0
#define TFT_INVERT 0

#define SQ 80

#define SDCARD_CS_PIN BUILTIN_SDCARD