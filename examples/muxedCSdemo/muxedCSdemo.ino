/*
 Example of multiplexed chip selects for multiple displays

 The test hardware uses a 74LVC138 with Y0 connected
 to a 2.25" 76x284 display, and Y1-3 to 3 1.54" 240x240
 displays. It should be evident from the code how to
 change this to suit your setup...
 */
#include <ST7789_t3.h> // Hardware-specific library

#define MUX_CS 0
#define MUX_A  1
#define MUX_B  2

#define GBL_RST  22
#define TFT_RST  -1 // shared - do reset in sketch code
#define TFT_DC   10
#define TFT_BLKL  4 // active low backlight (240x240 displays)
#define TFT_BLKH  3 // active high backlight (76x284 display)

/*
 * Use 74LVC138 decoder to provide /CS signal to one of
 * 4 displays, using only 3 Teensy outputs (we could 
 * address up to 8).
 */
static void CSfn(int which, bool negate)
{
  if (negate)
    digitalWriteFast(MUX_CS,1);
  else
  {
    digitalWriteFast(MUX_A,(which&1)!=0);
    digitalWriteFast(MUX_B,(which&2)!=0);
    // digitalWriteFast(MUX_C,(which&4)!=0); // use for >4 displays
    digitalWriteFast(MUX_CS,0);
  }
}

// Provide lambda functions to assert /CS for each display
ST7789_t3   ST7789a  = ST7789_t3( [](bool negate) { CSfn(3, negate); } , TFT_DC, TFT_RST);
ST7789_t3   ST7789b  = ST7789_t3( [](bool negate) { CSfn(2, negate); } , TFT_DC, TFT_RST);
ST7789_t3   ST7789c  = ST7789_t3( [](bool negate) { CSfn(1, negate); } , TFT_DC, TFT_RST);
ST7789_t3   ST7789d  = ST7789_t3( [](bool negate) { CSfn(0, negate); } , TFT_DC, TFT_RST);

#define SCREENS 4
ST7789_t3* screens[]{&ST7789a, &ST7789b, &ST7789c, &ST7789d}; // easier to deal with an array
uint16_t bkgnds[]{CL(24,0,0), CL(0,24,0), CL(0,0,24), CL(128,96,32)};


// Show we can address each screen individually
void identify(ST7789_t3& tft, int n, uint16_t bkgnd, int inv)
{
  char buf[20];
  int16_t  x,y;
  uint16_t w,h;

  // Basic settings:
  tft.setRotation(1);
  tft.invertDisplay(inv);
  tft.fillScreen(bkgnd);

  // Show screen number in the middle:
  sprintf(buf,"Screen #%d",n);
  tft.setTextColor(ST77XX_WHITE);    
  tft.setTextSize(3);
  tft.getTextBounds(buf,0,0,&x,&y,&w,&h);
  tft.setCursor((tft.width() - w)/2,(tft.height() - h)/2);
  tft.print(buf);

  // Show screen resolution at bottom right:
  sprintf(buf,"%d x %d",tft.width(), tft.height());
  tft.setTextSize(1);
  tft.getTextBounds(buf,0,0,&x,&y,&w,&h);
  tft.setCursor(tft.width() - w - 2,tft.height() - h - 2);
  tft.print(buf);
}

void setup() 
{
  // Initialise /CS multiplexer
#define SAFE_MUX(p,v) pinMode(p, OUTPUT); digitalWrite(p,v)
  SAFE_MUX(MUX_A,0);
  SAFE_MUX(MUX_B,0);
  SAFE_MUX(MUX_CS,1);

  // Backlights on:
  pinMode(TFT_BLKL, OUTPUT);
  pinMode(TFT_BLKH, OUTPUT);
  digitalWriteFast(TFT_BLKL,0);
  digitalWriteFast(TFT_BLKH,1);

  // Reset screens:
  pinMode(GBL_RST, OUTPUT);
  digitalWriteFast(GBL_RST,1); delay(1);
  digitalWriteFast(GBL_RST,0); delay(1);
  digitalWriteFast(GBL_RST,1); delay(1);

  // Initialise:
  for (int i=0;i<SCREENS-1;i++)
    screens[i]->init();
  screens[SCREENS-1]->init(76,284); // different, non-default size

  // Identify:
  for (int i=0;i<SCREENS;i++)
    identify(*screens[i], i, bkgnds[i], i!=3);
}

void loop() 
{
}
