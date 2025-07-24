#include <Arduino.h>

#include "config.h"
#include "utils.h"


void throbBacklight(int pin)
{
  int level = 0;
  bool up = true;

  pinMode(pin,OUTPUT);
  while (!Serial)
  {
    analogWrite(pin,level);
    if (up)
    {
      level++;
      if (level >= 254)
        up = false;
    }
    else
    {
      level--;
      if (level <= 1)
        up = true;
    }
    delay(3);
  }
}


void safeCSpins(void)
{
  const int CSpins[] = {28,29,30,31,32};
  for (int i =0;i<5;i++)
  {
    pinMode(CSpins[i],OUTPUT);
    digitalWrite(CSpins[i],1);
  }
}

// Paletted images stolen from KurtE's
// Kurts_ILI9341_t3n_FB_and_clip_tests.ino

uint16_t palette[256] = 
{
   ILI9341_BLACK , // 0x0000       /*   0,   0,   0 */
   ILI9341_NAVY , // 0x000F        /*   0,   0, 128 */
   ILI9341_DARKGREEN , // 0x03E0   /*   0, 128,   0 */
   ILI9341_DARKCYAN , // 0x03EF    /*   0, 128, 128 */
   ILI9341_MAROON , // 0x7800      /* 128,   0,   0 */
   ILI9341_PURPLE , // 0x780F      /* 128,   0, 128 */
   ILI9341_OLIVE , // 0x7BE0       /* 128, 128,   0 */
   ILI9341_LIGHTGREY , // 0xC618   /* 192, 192, 192 */
   ILI9341_DARKGREY , // 0x7BEF    /* 128, 128, 128 */
   ILI9341_BLUE , // 0x001F        /*   0,   0, 255 */
   ILI9341_GREEN , // 0x07E0       /*   0, 255,   0 */
   ILI9341_CYAN , // 0x07FF        /*   0, 255, 255 */
   ILI9341_RED , // 0xF800         /* 255,   0,   0 */
   ILI9341_MAGENTA , // 0xF81F     /* 255,   0, 255 */
   ILI9341_YELLOW , // 0xFFE0      /* 255, 255,   0 */
   ILI9341_WHITE , // 0xFFFF       /* 255, 255, 255 */
   ILI9341_ORANGE , // 0xFD20      /* 255, 165,   0 */
   ILI9341_GREENYELLOW , // 0xAFE5 /* 173, 255,  47 */
   ILI9341_PINK , // 0xF81F
   // wrap here
   ILI9341_BLACK , // 0x0000       /*   0,   0,   0 */
   ILI9341_NAVY , // 0x000F        /*   0,   0, 128 */
   ILI9341_DARKGREEN , // 0x03E0   /*   0, 128,   0 */
   ILI9341_DARKCYAN , // 0x03EF    /*   0, 128, 128 */
   ILI9341_MAROON , // 0x7800      /* 128,   0,   0 */
   ILI9341_PURPLE , // 0x780F      /* 128,   0, 128 */
   ILI9341_OLIVE , // 0x7BE0       /* 128, 128,   0 */
   ILI9341_LIGHTGREY , // 0xC618   /* 192, 192, 192 */
   ILI9341_DARKGREY , // 0x7BEF    /* 128, 128, 128 */
   ILI9341_BLUE , // 0x001F        /*   0,   0, 255 */
   ILI9341_GREEN , // 0x07E0       /*   0, 255,   0 */
   ILI9341_CYAN , // 0x07FF        /*   0, 255, 255 */
   ILI9341_RED , // 0xF800         /* 255,   0,   0 */
   ILI9341_MAGENTA , // 0xF81F     /* 255,   0, 255 */
   ILI9341_YELLOW , // 0xFFE0      /* 255, 255,   0 */
   ILI9341_WHITE , // 0xFFFF       /* 255, 255, 255 */
   ILI9341_ORANGE , // 0xFD20      /* 255, 165,   0 */
   ILI9341_GREENYELLOW , // 0xAFE5 /* 173, 255,  47 */
   ILI9341_PINK , // 0xF81F
  };  // 19 colours, + wrap

//========================================================
void setRGB(float a, uint8_t& r, uint8_t& g, uint8_t& b)
{
  if (a<0) a=1.0f+a;
  float aa=a, p=0.5f;
  
  a=1-a; a=1-a*a; // quadratic
  //a=sin(a*PI/2); // sine

  a=a*p+aa*(1-p);
  r=255; g=a*255; b=0;
}


void baseColour(float a, uint8_t& r, uint8_t& g, uint8_t& b)
{
  uint8_t ia=(int) a;
  a -= ia;
  
  switch (ia)
  {
    case 0: setRGB( a,r,g,b); break; // 0-1
    case 1: setRGB(-a,g,r,b); break; // 1-2
    case 2: setRGB( a,g,b,r); break; // 2-3
    case 3: setRGB(-a,b,g,r); break; // 3-4
    case 4: setRGB( a,b,r,g); break; // 4-5
    case 5: setRGB(-a,r,b,g); break; // 5-6
  }
}




//////////////////////////////////////////////////////////////
//
//           888                        888               
//           888                        888               
//           888                        888               
//   .d8888b 88888b.   .d88b.   .d8888b 888  888 .d8888b  
//  d88P"    888 "88b d8P  Y8b d88P"    888 .88P 88K      
//  888      888  888 88888888 888      888888K  "Y8888b. 
//  Y88b.    888  888 Y8b.     Y88b.    888 "88b      X88 
//   "Y8888P 888  888  "Y8888   "Y8888P 888  888  88888P' 
//
//////////////////////////////////////////////////////////////
DMAMEM uint16_t save1[SQ*SQ], save2[SQ*SQ];

uint16_t colours[] = {ST7735_RED, CL(255,128,0), ST7735_YELLOW, ST7735_GREEN, ST7735_CYAN, ST7735_BLUE, ST7735_MAGENTA, ST7735_WHITE};

//------------------------------------------------------------------------
uint16_t nextColour(bool inc)
{
  static int idx;
  uint16_t result = colours[idx];

  if (inc) idx++;
  if (idx >= COUNT_OF(colours))
    idx = 0;

  return result;    
}

elapsedMicros checkMicros;
template<class TFT> uint32_t check_triangles(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.setOrigin(x,y);
  int hw = SQ*3/4-2, hh = SQ/2-2;

  tft.fillTriangle(0,0,    hw*2,0,  hw*2,hh*2, nextColour());
  tft.fillTriangle(0,0,    0,hh*2,  hw,hh,     nextColour());
  tft.fillTriangle(0,hh*2, hw,hh,   hw*2,hh*2, nextColour());

  tft.setOrigin();

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_fillRect(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.fillRect(x,y,SQ,SQ,nextColour());

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_fillRectX4(TFT& tft, int x, int y)
{
  checkMicros = 0;
  uint16_t c = nextColour();

  for (int i=0;i<5;i++)
    tft.fillRect(x+i*SQ/5,y,SQ/5,SQ,c);

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_fillVGradient(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.fillRectVGradient(x,y,20,200,nextColour(),nextColour());

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_fillHGradient(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.fillRectHGradient(x,y,200,20,nextColour(),nextColour());
  //tft.fillRectHGradient(SQ*5/4,SQ*13/4,SQ*4+17,SQ/2,0x0000,CL(40,40,40));

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_drawChar(TFT& tft, int x, int y)
{
  tft.fillRect(x,y,96,SQ,nextColour());
  checkMicros = 0;

  tft.setFont();
  tft.setTextSize(8);
  tft.setTextColor(nextColour());
  tft.setCursor(x,y);
  tft.print("qb");

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_drawChar_bg(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.setFont();
  tft.setTextSize(8);
  tft.setTextColor(nextColour(),ST7735_BLACK);
  tft.setCursor(x,y);
  tft.print("t8");

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_drawFontChar(TFT& tft, int x, int y)
{
  tft.fillRect(x,y,96,90,nextColour());
  checkMicros = 0;

  tft.setFont(Arial_60);
  tft.setTextColor(nextColour());
  tft.setCursor(x,y);
  tft.print("qb");

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_drawFontChar_bg(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.setFont(Arial_60);
  tft.setTextColor(nextColour(),0x38E7);
  tft.setCursor(x,y);
  tft.print(" t8 ");

  return checkMicros;
}
//------------------------------------------------------------------------
/* // Doesn't work, and isn't really supposed to!
template<class TFT> uint32_t check_drawAAChar(TFT& tft, int x, int y)
{
  tft.fillRect(SQ+96,SQ+96,96,90,nextColour());
  checkMicros = 0;

  tft.setFont(OpenSans60);
  tft.setTextColor(nextColour());
  tft.setCursor(SQ+96,SQ+96);
  tft.print("qb");

  return checkMicros;
}
*/
//------------------------------------------------------------------------
template<class TFT> uint32_t check_drawAAChar_bg(TFT& tft, int x, int y)
{
  checkMicros = 0;

  tft.setFont(OpenSans60);
  tft.setTextColor(nextColour(),0x38E7);
  tft.setCursor(x,y);
  tft.print(" t8 ");

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_writeRect(TFT& tft, int x, int y)
{
  checkMicros = 0;
  static int which;

  uint16_t* wr = (which&16)?save1:save2; // only change every 16 calls
  tft.writeRect(x,y,SQ,SQ,wr);
  which++;// = !which;

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_writeSubImageRect(TFT& tft, int x, int y)
{
  tft.fillRect(x,y,SQ,SQ,nextColour()); // don't count this
  
  checkMicros = 0;
  static int which;
  int off = 8;

  uint16_t* wr = (which&32)?save1:save2;
  tft.writeSubImageRect(x+off,y+off,SQ-off*2,SQ-off*2,
                        off, off, SQ, SQ,
                        wr);
  which++;// = !which;

  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_writeSubImageRectBytesReversed(TFT& tft, int x, int y)
{
  tft.fillRect(x,y,SQ,SQ,nextColour()); // don't count this
  
  checkMicros = 0;
  static int which;
  int off = 8;

  uint16_t* wr = (which&8)?save1:save2;
  tft.writeSubImageRect(x+off,y+off,SQ-off*2,SQ-off*2,
                        off, off, SQ, SQ,
                        wr);
  which++;// = !which;

  return checkMicros;
}

int palIdx;
void nextPalette(void)
{
  palIdx++;
  if (palIdx >= 19)
    palIdx = 0;
}
//------------------------------------------------------------------------
extern uint8_t img1bpp80x80[];
template<class TFT> uint32_t check_writeRect1BPP(TFT& tft, int x, int y)
{  
  checkMicros = 0;
  nextPalette();
  tft.writeRect1BPP(x, y, 80, 80, &img1bpp80x80[0], palette + palIdx);
  // tft.writeRect1BPP(SQ, SQ+96, 16, 16, pict1bpp, palette + palIdx);
  return checkMicros;
}

//------------------------------------------------------------------------
extern uint8_t img2bpp80x80[];
template<class TFT> uint32_t check_writeRect2BPP(TFT& tft, int x, int y)
{  
  checkMicros = 0;
  nextPalette();
  tft.writeRect2BPP(x, y, 80, 80, &img2bpp80x80[0], palette + palIdx);
  //tft.writeRect2BPP(SQ*2, SQ+96, 32, 16, pict2bpp, palette + palIdx);  
  return checkMicros;
}

//------------------------------------------------------------------------
extern uint8_t img4bpp80x80[];
template<class TFT> uint32_t check_writeRect4BPP(TFT& tft, int x, int y)
{  
  checkMicros = 0;
  // tft.writeRectNBPP(SQ*3, SQ+96, 16, 16, 4, pict4bpp, palette);
  nextPalette();
  tft.writeRectNBPP(x, y, 80, 80, 4, &img4bpp80x80[0], palette + palIdx);
  return checkMicros;
}

//------------------------------------------------------------------------
template<class TFT> uint32_t check_updateScreen(TFT& tft, int x, int y)
{  
  checkMicros = 0;
  tft.updateScreen();
  return checkMicros;
}

//------------------------------------------------------------------------
int whichBox;

template<class TFT> uint32_t check_updateClip(TFT& tft, int x, int y)
{  
  checkMicros = 0;
  switch (whichBox)
  {
    default:
      whichBox = -1; // finish
      break;

    case 0:
      tft.setClipRect(40,40,80,80);
      break;

    case 1:
      tft.setClipRect(60,200,200,40);
      break;

    case 2:
      tft.setClipRect(0,0,397,40);
      break;

    case 3:
      tft.setClipRect(40,120,359,80);
      break;

    case 4:
      tft.setClipRect(0,40,40,280);
      break;

    case 5:
      tft.setClipRect(120,40,279,80);
      break;

    case 6:
      tft.setClipRect(260,200,67,57);
      break;
  
    case 7:
      tft.setClipRect(40,200,40,120);
      break;
  
    case 8:
      tft.setClipRect(80,240,180,17);
      break;

    case 9:
      tft.setClipRect(SQ*5/4,SQ*13/4,SQ*4+17,SQ/2);
      break;

    case 10:
      tft.setClipRect(SQ*5+2,2,SQ-4,SQ*3-4);
      break;
  }

  if (whichBox >= 0)
  {
    tft.updateScreenAsync(false,true,true);
  }
  whichBox++;
  return whichBox-1;
}



