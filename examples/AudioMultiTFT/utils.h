//
#if !defined(_UTILS_H_)
#define _UTILS_H_
extern uint16_t nextColour(bool inc = true);
extern void safeCSpins(void);
extern void throbBacklight(int pin);

template<class TFT>
size_t allocateFB(TFT& tft, bool inPSRAM = false, bool allocate = true)
{
  size_t result = tft.width()*tft.height()*2;
  if (allocate)
  {
    uint16_t* fb;
    if (inPSRAM)
      fb = (uint16_t*) extmem_malloc(result);
    else
      fb = (uint16_t*) malloc(result);

    if (nullptr == fb)
      result = 0; // nothing allocated!
    else
      tft.setFrameBuffer(fb);

  }
  return result;
}

extern void setRGB(float a, uint8_t& r, uint8_t& g, uint8_t& b);
extern void baseColour(float a, uint8_t& r, uint8_t& g, uint8_t& b);

template<class TFT>
void showGamut(TFT& tft)
{
  int w=tft.width(), h=tft.height();
  int hw=w/2, hh=h/2;
  //int hw2=hw*hw, hh2=hh*hh;

  tft.fillScreen(0);

  // top edge
  for (int x=0; x<w; x++)
  {
    int xo=x-hw;
    float a=atan2(xo,hh)/PI*3 + 3; // divide into 6 sectors
    uint8_t r,g,b;
    
    baseColour(a,r,g,b);
    
    for (int y=0;y<=hh;y++)
    {
      int xp=hw+(x-hw)*(hh-y)/hh;
      uint8_t pr,pg,pb,wh;
      wh=(255*y)/hh;
      pr = (r*(hh-y))/hh + wh;
      pg = (g*(hh-y))/hh + wh;
      pb = (b*(hh-y))/hh + wh;

      tft.drawPixel(xp,y,CL(pr,pg,pb));
    }
  }

  // right edge
  for (int y=0; y<h; y++)
  {
    int yo=hh-y;
    float a=atan2(hw,yo)/PI*3 + 3; // divide into 6 sectors
    uint8_t r,g,b;
    
    baseColour(a,r,g,b);
    
    for (int x=0;x<=hw;x++)
    {
      int yp=hh+(y-hh)*(hw-x)/hw;
      uint8_t pr,pg,pb,wh;
      wh=(255*x)/hw;
      pr = (r*(hw-x))/hw + wh;
      pg = (g*(hw-x))/hw + wh;
      pb = (b*(hw-x))/hw + wh;

      tft.drawPixel(w-x-1,yp,CL(pr,pg,pb));
    }
  }
  
  // bottom edge
  for (int x=w-1; x>=0; x--)
  {
    int xo=x-hw;
    float a=atan2(xo,-hh)/PI*3 + 3; // divide into 6 sectors
    uint8_t r,g,b;
    
    baseColour(a,r,g,b);
    
    for (int y=0;y<=hh;y++)
    {
      int xp=hw+(x-hw)*(hh-y)/hh;
      uint8_t pr,pg,pb,wh;
      wh=(255*y)/hh;
      pr = (r*(hh-y))/hh + wh;
      pg = (g*(hh-y))/hh + wh;
      pb = (b*(hh-y))/hh + wh;

      tft.drawPixel(xp,h-y-1,CL(pr,pg,pb));
    }
  }

  // left edge
  for (int y=0; y<=h; y++)
  {
    int yo=y-hh;
    float a=atan2(-hw,yo)/PI*3 + 3; // divide into 6 sectors
    uint8_t r,g,b;
    
    baseColour(a,r,g,b);
    
    for (int x=0;x<=hw;x++)
    {
      int yp=hh+(hh-y)*(hw-x)/hw;
      uint8_t pr,pg,pb,wh;
      wh=(255*x)/hw;
      pr = (r*(hw-x))/hw + wh;
      pg = (g*(hw-x))/hw + wh;
      pb = (b*(hw-x))/hw + wh;

      tft.drawPixel(x,yp,CL(pr,pg,pb));
    }
  }
}


template<class TFT>
void gamutFB(TFT& tft)
{
  tft.useFrameBuffer(true);
  showGamut<TFT>(tft);
  tft.updateScreen();
  tft.useFrameBuffer(false);
}

#endif // defined(_UTILS_H_)