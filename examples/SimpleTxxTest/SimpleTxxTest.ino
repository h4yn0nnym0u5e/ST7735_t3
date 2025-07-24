#include <ST7789_t3.h> // based on ST7735, so we have that too
#include <ST7796_t3.h> 
#include <ILI9341_t3n.h>
#include <GC9A01A_t3n.h>

#include "config.h"

ILI9341_t3n ILI9341 = ILI9341_t3n(ILI9341_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
ST7735_t3   ST7735  = ST7735_t3(ST7735_CS, TFT_DC);
ST7789_t3   ST7789  = ST7789_t3(ST7789_CS, TFT_DC, TFT_RST);
ST7796_t3   ST7796  = ST7796_t3(ST7796_CS, TFT_DC, TFT_RST);
GC9A01A_t3n GC9A01A = GC9A01A_t3n(GC9A01_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK);

void initScreens(void)
{
    digitalWrite(GBL_BLK,1);

    digitalWrite(GBL_RST,1); delay(1);
    digitalWrite(GBL_RST,0); delay(1);
    digitalWrite(GBL_RST,1); delay(10);

    GC9A01A.begin();
    ILI9341.begin();
    ST7735.initR(INITR_BLACKTAB);
    ST7789.init(240,240);
    ST7796.init(320,480);

    ILI9341.invertDisplay(false);
    ILI9341.setRotation(1);
    GC9A01A.invertDisplay(true);
    ST7796.setRotation(1);
}

void setup(void)
{
    initScreens();
}

void loop(void)
{
    
}