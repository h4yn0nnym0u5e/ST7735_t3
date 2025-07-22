/*
 * Multi-screen test with audio
 */
#include <Audio.h>
#include <TeensyDebug.h>

#include <ST7789_t3.h> // based on ST7735, so we have that too
#include <ST7796_t3.h> 
#include <ILI9341_t3n.h>
#include <GC9A01A_t3n.h>

#include "config.h"
#include "utils.h"
#define COUNT_OF(a) (int)(sizeof a / sizeof a[0])

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=136,65
AudioAnalyzePeak         peak2;          //xy=348,219
AudioAnalyzePeak         peak1;          //xy=358,171
AudioOutputI2S           i2s1;           //xy=380,92
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 0, peak1, 0);
AudioConnection          patchCord3(playSdWav1, 1, i2s1, 1);
AudioConnection          patchCord4(playSdWav1, 1, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=155,192
// GUItool: end automatically generated code

ILI9341_t3n ILI9341 = ILI9341_t3n(ILI9341_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
ST7735_t3   ST7735  = ST7735_t3(ST7735_CS, TFT_DC);
ST7789_t3   ST7789  = ST7789_t3(ST7789_CS, TFT_DC, TFT_RST);
ST7796_t3   ST7796  = ST7796_t3(ST7796_CS, TFT_DC, TFT_RST);
GC9A01A_t3n GC9A01A = GC9A01A_t3n(GC9A01_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK);

//=================================================================================
extern uint16_t save1[], save2[];
void makeSave(ST7735_t3& tft, uint16_t* sv, int n)
{
  uint16_t bg = ST7735_BLACK;
  const char* msg;
  
  switch (n)
  {
    default:
      tft.setTextColor(ST7735_RED);
      msg = "on";
      break;

    case 1:
      tft.setTextColor(ST7735_GREEN);
      bg = ST7735_BLUE;
      msg = "off";
      break;         
  }

  tft.fillRect(0,0,SQ,SQ,bg);
  tft.setFont(Arial_24);
  tft.setCursor(25,25);
  tft.print(msg);
  tft.readRect(0,0,SQ,SQ,sv);
  //delay(500);
}


void makeSaves(ST7735_t3& tft)
{
    tft.useFrameBuffer(true);
    makeSave(tft,&save1[0],0);
    makeSave(tft,&save2[0],1);
    tft.useFrameBuffer(false);
    tft.freeFrameBuffer();
}

//=================================================================================
void initAudio(void)
{
  AudioMemory(10);
// sgtl5000_1.setAddress(HIGH);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  while (!(SD.begin(SDCARD_CS_PIN))) 
  {
      Serial.println("Unable to access the SD card");
      delay(500);
  }
}

void playAudio(void)
{
    if (playSdWav1.isPlaying() == false) 
    {
      Serial.println("Start playing");
      //playSdWav1.play("SDTEST1.WAV");
      //playSdWav1.play("SDTEST2.WAV");
      //playSdWav1.play("SDTEST3.WAV");
      playSdWav1.play("SDTEST4.WAV");
      delay(10); // wait for library to parse WAV info
    }  
}

//=================================================================================
//                    888                      
//                    888                      
//                    888                      
//  .d8888b   .d88b.  888888 888  888 88888b.  
//  88K      d8P  Y8b 888    888  888 888 "88b 
//  "Y8888b. 88888888 888    888  888 888  888 
//       X88 Y8b.     Y88b.  Y88b 888 888 d88P 
//   88888P'  "Y8888   "Y888  "Y88888 88888P"  
//                                    888      
//                                    888      
//                                    888      
//=================================================================================
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


extern "C" uint32_t dma_channel_allocated_mask;
uint32_t lastMask = 0xFFFF'FFFF;
void DMAcheck(void)
{
    if (lastMask != dma_channel_allocated_mask)
    {
        lastMask = dma_channel_allocated_mask;
        Serial.printf("Allocated DMA channels: %08X\n",lastMask);
    }
}


#define ALLOC_FB(tft,tftx,ps) Serial.printf("Allocated %dkB for %s\n",allocateFB<tft##tftx>(tft,ps)/1024,#tft)
void setup(void)
{
    
    pinMode(GBL_BLK,OUTPUT);
    pinMode(GBL_RST,OUTPUT);
    safeCSpins();

    while (!Serial)
    {
        digitalToggleFast(GBL_BLK);
        delay(100);
    }
    
    initScreens();

    makeSaves(ST7735);

    initAudio();
    playAudio();

    showGamut<GC9A01A_t3n>(GC9A01A);
    showGamut<ST7735_t3>(ST7735);
    showGamut<ST7789_t3>(ST7789);
    showGamut<ST7796_t3>(ST7796);
    showGamut<ILI9341_t3n>(ILI9341); 
    
    halt_cpu();

    ALLOC_FB(ST7735,_t3,false);
    ALLOC_FB(ST7789,_t3,false);
    ALLOC_FB(ST7796,_t3,true);

    DMAcheck();
    ST7796.useFrameBuffer(true);
    showGamut<ST7796_t3>(ST7796);
    ST7796.useFrameBuffer(false);
    //ST7796.setMaxAsyncLines(2);
    ST7796.setAsyncInterruptPriority(224);

    ST7789.useFrameBuffer(true);
    showGamut<ST7789_t3>(ST7789);
    ST7789.useFrameBuffer(false);
    ST7789.setMaxAsyncLines(5);

}


//=================================================================================
//  888                            
//  888                            
//  888                            
//  888  .d88b.   .d88b.  88888b.  
//  888 d88""88b d88""88b 888 "88b 
//  888 888  888 888  888 888  888 
//  888 Y88..88P Y88..88P 888 d88P 
//  888  "Y88P"   "Y88P"  88888P"  
//                        888      
//                        888      
//                        888
//=================================================================================
void run_check_async(const char* tft, const char* chk,
                     int x, int y,
                     uint32_t (*chkFn)(int,int))
{
    DMAcheck();
    Serial.printf("%-8s@ %-18s - %6d\n",tft,chk,chkFn(x,y));
}

void time_async(bool (*updFn)(void))
{
    elapsedMicros t = 0;
    bool ok = updFn();
    uint32_t t2 = t;
    Serial.printf("Async was %sOK: took %dus\n",ok?"":"not ",t2);
}

// #define RUN_CHECK(chk,tft,tftx,x,y) check_##chk<tft##tftx>(tft,x,y)
#define RUN_CHECK(chk,tft,tftx,x,y) \
    DMAcheck(); Serial.printf("%-8s: %-18s - %6d\n",#tft,#chk,check_##chk<tft##tftx>(tft,x,y))

#define RUN_CHECK_ASYNC(chk,tft,tftx,x,y) \
    tft.useFrameBuffer(true); \
    run_check_async(#tft,#chk,x,y,[](int a, int b){ return check_##chk<tft##tftx>(tft,a,b);}); \
    time_async([](){ bool ok = tft.updateScreenAsync(false,true); tft.waitUpdateAsyncComplete(); return ok;}); \
    tft.useFrameBuffer(false)

void runChecks(void)
{
    static int checkCount = 0;
    uint16_t lastColour = nextColour(false);

    Serial.printf("Check #%d\n",++checkCount);
    RUN_CHECK_ASYNC(drawAAChar_bg,ST7789,_t3,34,84);
    RUN_CHECK(fillHGradient,ST7789,_t3,20,20);

    RUN_CHECK_ASYNC(drawFontChar_bg,ST7796,_t3,300,180);
    RUN_CHECK(drawFontChar,ST7796,_t3,80,80);

    RUN_CHECK(fillRectX4,ILI9341,_t3n,115,10);
    RUN_CHECK(fillVGradient,GC9A01A,_t3n,110,20);
    RUN_CHECK(writeSubImageRect,ST7735,_t3,14,0);

    //RUN_CHECK(drawFontChar_bg,ST7796,_t3,300,180);

    if (lastColour == nextColour(false))
        nextColour();
    Serial.println();        
}


void loop(void)
{
    if (Serial.available())
    {
        while (Serial.read() != -1)
            ;
        initScreens();            
    }

    playAudio();
    runChecks();
    delay(10);
}
