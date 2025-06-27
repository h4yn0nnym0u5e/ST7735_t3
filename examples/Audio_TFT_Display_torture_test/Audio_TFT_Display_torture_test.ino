/*
 * Use as many different display output methods as possible,
 * which each take a long time to execute on the SPI bus,
 * while playing audio from the SD card (which claims to
 * use SPI, although in fact it doesn't).
 */
//------------------------------------------------------------
// Pick your TFT here:
/*
#include <ILI9341_t3n.h>
/*/
#include <ST7796_t3.h>
//*/

/* 
 * Pick an update mode: 
 *  0 = immediate
 *  1 = frame buffer 
 *  2 = async frame buffer
 *  3 = async frame buffer, continuous
 *  4 = async frame buffer; do in one DMA request
 *  5 = async frame buffer in PSRAM
 *  6 = async frame buffer, IRQ every chunk
 *  7 = async frame buffer, IRQ every chunk, continuous
 *  8 = async frame buffer, clipped
 *  9 = async frame buffer, update changed range
 * 10 = async frame buffer in PSRAM, update changed range
 * 11 = async frame buffer, update clip rectangle, continuous
 * 12 = async frame buffer, continuous, changing clip rectangle
 * 13 = async frame buffer, continuous, update changed range
 */
#define UPDATE_MODE 13
#define notMICRO_DEXED
#define notMINI_PLATFORM

const char* sbok="should be OK", 
           *xbrk="expected to be broken",
           *oops="says 'you're a numpty'";
const char* auok[] = 
{
  sbok, sbok, xbrk, xbrk, xbrk, // 0-4
  xbrk, sbok, sbok, sbok, sbok, // 5-9
  sbok, sbok, sbok, sbok,       // 10-13

  oops, oops, oops, oops, oops, // oh dear...
};
//------------------------------------------------------------

#if defined ST77XX_BLACK // see which TFT we're using
  #include <ST7735_t3_font_Arial.h>
  #include <ST7735_t3_font_OpenSans.h>
#else
  #if defined(_ILI9341_t3NH_)
    #include <ili9341_t3n_font_Arial.h>    // from ILI9341_t3n
    #include <ili9341_t3n_font_OpenSans.h> // from ILI9341_t3n
  #else
    #include <font_Arial.h> // from ILI9341_t3
  #endif // defined(_ILI9341_t3NH_)
  
  #define ST7735_BLACK ILI9341_BLACK
  #define ST7735_RED ILI9341_RED
  #define ST7735_YELLOW ILI9341_YELLOW
  #define ST7735_GREEN ILI9341_GREEN
  #define ST7735_CYAN ILI9341_CYAN
  #define ST7735_BLUE ILI9341_BLUE
  #define ST7735_MAGENTA ILI9341_MAGENTA
  #define ST7735_WHITE ILI9341_WHITE
  //#define ST7735_BLACK ILI9341_BLACK
#endif // defined ST77XX_BLACK


#define COUNT_OF(a) ((int)(sizeof a / sizeof a[0]))


#include <Audio.h>
//DMAChannel dummy;
///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////
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

//#include <TeensyDebug.h>

#if defined(MICRO_DEXED)
  #define TFT_DC      37
  #define TFT_CS      41
  #define TFT_MOSI    26
  #define TFT_MISO   255
  #define TFT_SCLK    27
  #define TFT_RST     24  // 255 = unused, connect to 3.3V

  #define LED_PWM 255 // used to set brightness of LED backlight
  #define ROTATE 3
  #define INVERT_DISPLAY true
#else
  // Use these with the Teensy 4.x and Audio Shield Rev D or D2
  // these are h4yn0nnymou5e pin assignments - you may need to change them
  #define TFT_DC       9
  #define TFT_CS      22
  #define TFT_RST    255  // 255 = unused, connect to 3.3V

  #define LED_PWM  4 // used to set brightness of LED backlight
  #define ROTATE   1
  #define INVERT_DISPLAY false
#endif // defined(MICRO_DEXED)

#if defined(MINI_PLATFORM)
// https://protosupplies.com/product/mini-platform-teensy41/
  #undef TFT_CS
  #undef LED_PWM
  #undef ROTATE
  #undef INVERT_DISPLAY

  #define TFT_CS    10
  #define ROTATE     3
  #define LED_PWM  255
  #define INVERT_DISPLAY true
#endif // defined(MINI_PLATFORM)



#if defined ST77XX_BLACK
  ST7796_t3 tft = ST7796_t3(TFT_CS, TFT_DC);
  #define ILI9341_BLACK 0x0000       /*   0,   0,   0 */
  #define ILI9341_NAVY 0x000F        /*   0,   0, 128 */
  #define ILI9341_DARKGREEN 0x03E0   /*   0, 128,   0 */
  #define ILI9341_DARKCYAN 0x03EF    /*   0, 128, 128 */
  #define ILI9341_MAROON 0x7800      /* 128,   0,   0 */
  #define ILI9341_PURPLE 0x780F      /* 128,   0, 128 */
  #define ILI9341_OLIVE 0x7BE0       /* 128, 128,   0 */
  #define ILI9341_LIGHTGREY 0xC618   /* 192, 192, 192 */
  #define ILI9341_DARKGREY 0x7BEF    /* 128, 128, 128 */
  #define ILI9341_BLUE 0x001F        /*   0,   0, 255 */
  #define ILI9341_GREEN 0x07E0       /*   0, 255,   0 */
  #define ILI9341_CYAN 0x07FF        /*   0, 255, 255 */
  #define ILI9341_RED 0xF800         /* 255,   0,   0 */
  #define ILI9341_MAGENTA 0xF81F     /* 255,   0, 255 */
  #define ILI9341_YELLOW 0xFFE0      /* 255, 255,   0 */
  #define ILI9341_WHITE 0xFFFF       /* 255, 255, 255 */
  #define ILI9341_ORANGE 0xFD20      /* 255, 165,   0 */
  #define ILI9341_GREENYELLOW 0xAFE5 /* 173, 255,  47 */
  #define ILI9341_PINK 0xF81F
#else
  #if defined(_ILI9341_t3NH_)
    #if  defined(MICRO_DEXED)
      ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
    #else
      ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC);//, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
    #endif // defined(MICRO_DEXED)
  #else
    ILI9341_t3  tft = ILI9341_t3(TFT_CS, TFT_DC);//, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
  #endif // defined(_ILI9341_t3NH_)
#endif // defined ST77XX_BLACK

// Use this with the Teensy 3.5 & 3.6 & 4.1 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD


//=============================================================================
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
uint16_t pixel_data[2500];
const uint8_t pict1bpp[] = {
  0xff, 0xff, 0xc0, 0x03, 0xa0, 0x05, 0x90, 0x9, 
  0x88, 0x11, 0x84, 0x21, 0x82, 0x41, 0x81, 0x81,
  0x81, 0x81, 0x82, 0x41, 0x84, 0x21, 0x88, 0x11, 
  0x90, 0x09, 0xa0, 0x05, 0xc0, 0x03, 0xff, 0xff
};
const uint8_t pict2bpp[] = {
  0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff,
  0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff,
  0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 
  0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 
  0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 
  0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 
  0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 
  0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 
  0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff,
  0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff,
  0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 
  0x55, 0x55, 0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 
  0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 
  0xaa, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 
  0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 
  0xff, 0xff, 0x00, 0x00, 0x55, 0x55, 0xaa, 0xaa, 
};
const uint8_t pict4bpp[] = {  
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00,  
  0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00,  
  0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x33, 0x33, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x33, 0x33, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x33, 0x33, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x33, 0x33, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x22, 0x22, 0x22, 0x22, 0x11, 0x00,
  0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00,  
  0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00,  
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/*
void initPalette(void)
{
  palette[0] = ILI9341_CYAN; 
  palette[1] = ILI9341_OLIVE;  
  palette[2] = ILI9341_MAROON; 
  palette[3] = ILI9341_PINK;   
}
*/

//=============================================================================


#define SQ 80
DMAMEM uint16_t save1[SQ*SQ], save2[SQ*SQ];

void makeSave(uint16_t* sv, int n)
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


void fillGrid(int pitch, uint16_t colour = ST77XX_WHITE)
{
  for (int i=0;i<tft.width();i+=pitch)
    tft.drawLine(i,0,i,tft.height()-1,colour);
  for (int i=0;i<tft.height();i+=pitch)
    tft.drawLine(0,i,tft.width()-1,i,colour);
  tft.drawRect(0,0,tft.width(),tft.height(),colour);
}


//=================================================================================
// Some of this may not work if classes have their proper 
// public / protected / private settings in place
#define noCLASSES_UNPROTECTED 
void printSetup(void)
{
  bool has_DMA_preemption = 
#if defined(DMA_DCHPRI_DPA) // we have pre-emption capability
  true 
#else
  false  
#endif // defined(DMA_DCHPRI_DPA)  
  ; 

#if defined(CLASSES_UNPROTECTED) // only available if protection removed
  Serial.printf("Frame buffer: %08X; intermediate buffer: %08X (%d)\n",
    tft._pfbtft, tft._intbData, tft._intbSize);
#endif // defined(CLASSES_UNPROTECTED)

  Serial.printf("DMA pre-emption is %spossible\n",
                has_DMA_preemption?"":"not ");

  const char* audioOK = auok[UPDATE_MODE];
  if (8 == UPDATE_MODE && !has_DMA_preemption)
    audioOK = "requires DMA pre-emption to work";
  Serial.printf("Update mode %d; audio playback from SD %s\n",
      UPDATE_MODE, audioOK);
}

extern uint32_t dma_channel_allocated_mask;
void printDMAchannel(void)
{
#if defined(CLASSES_UNPROTECTED)
  static bool done = false;

  if (!done && 0 != (tft._dma_state & 0x01 /* ST77XX_DMA_INIT */))
  {
    // Show DMA channel allocation mask
    Serial.printf("DMA channel allocation: %08X\n", dma_channel_allocated_mask);

    auto priBase = &DMA_DCHPRI3; // very weird register order: see 6.5.5.18
    uint8_t channel = tft._dma_data[0]._dmatx.channel;
    NVIC_SET_PRIORITY(channel&15,224); // force lower priority
    Serial.printf("SPI DMA is using channel %d; DMA priority %02X; ISR priority %d\n",
      channel,
      priBase[channel+3 - 2*(channel&3)],
      NVIC_GET_PRIORITY(channel & 15));
  
    done = true;
  }
#endif // defined(CLASSES_UNPROTECTED)
}

//---------------------------------------------------------------------------------
void fbInPSRAM(void)
{
  uint16_t* fb = (uint16_t*) extmem_malloc(tft.width() * tft.height() * 2);
  if (nullptr != fb)
  {
    Serial.println("Allocated frame buffer in PSRAM");
    tft.setFrameBuffer(fb);
  }
}

//---------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------
void setup() {
  pinMode(LED_PWM, OUTPUT);

  // debug pins
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  
  //analogWrite(LED_PWM, 64);
  
  {
    bool blOn = false;
    while (!Serial)
    {
      blOn = !blOn;
      digitalWrite(LED_PWM,blOn);
      delay(250);
    }
    if (CrashReport)
      Serial.print(CrashReport);
  }
  digitalWrite(LED_PWM,1);
    
  Serial.begin(9600);
  delay(100);
  //tft.setClock(16000000);
    // Setup the LCD screen
#if defined ST77XX_BLACK
  tft.init(320, 480);
  tft.setRotation(ROTATE);         // Rotates screen to match the baseboard orientation
#if defined(TEENSY_DEBUG_H)
  halt_cpu();
#endif // defined(TEENSY_DEBUG_H)
  
  // Set up frame complete callback.
  // Do this before we set the interrupt priority, because
  // that initialises DMA and this clears the DMA init flag,
  // for some reason 
  tft.setFrameCompleteCB(frameCompleteFn, true);

  // Lower the DMA interrupt priority so audio works OK.
  // This MUST be done before the first attempt to do
  // an asynchronous screen update.
  tft.setAsyncInterruptPriority(224); 

  // 16MHz SPI is ~1us / pixel, so a 480 pixel line is ~480us
  // 40MHz       400ns                                 ~192us
  /*/
  tft.setSPISpeed(40'000'000);
  tft.setMaxAsyncLines(5);   //  64 updates of 5 lines each, ~960us
  /*/
  tft.setMaxAsyncLines(2);   // 160 updates of 2 lines each, ~960us
  //*/
#else  
  tft.begin();
  tft.setRotation(ROTATE);       // Rotates screen to match the baseboard orientation
#endif // defined ST77XX_BLACK

  //initPalette();

  tft.invertDisplay(INVERT_DISPLAY);  // LCD may require colors to be inverted
  tft.setTextWrap(false);

  tft.fillScreen(ST7735_BLACK);
  fillGrid(40);


  tft.useFrameBuffer(true);
  makeSave(&save1[0],0);
  makeSave(&save2[0],1);
  tft.useFrameBuffer(false);
  tft.freeFrameBuffer();
  
  AudioMemory(10);
// sgtl5000_1.setAddress(HIGH);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  while (!(SD.begin(SDCARD_CS_PIN))) 
  {
      Serial.println("Unable to access the SD card");
      delay(500);
  }

#if defined ST77XX_BLACK
  //tft.setMaxTransaction(100000);  // deliberately break SD playback!
  tft.setMaxTransaction(1400);      // default is 1000, but this should be OK
  tft.enableYieldInMidTransaction(true); // does a bonus yield() if a mid-transaction break occurs
#endif // defined ST77XX_BLACK

  switch (UPDATE_MODE)
  {
    case 5:
      fbInPSRAM();
      // fallthrough
      case 2:
      case 3:
      tft.setMaxAsyncLines(320);
      // fallthrough    
    default:
      tft.useFrameBuffer(true);
      break;

    case 10:
      fbInPSRAM();
      // fallthrough
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
      {
        // two-line (480x2) intermediate buffer, 
        // i.e. 960 pixels, or 30x32 pixels, etc.
        tft.useIntermediateBuffer(tft.width() * 2 * 2);
        tft.useFrameBuffer(true);
      }
      break;

    case 0:
      break;
  }
  fillGrid(40);

  printSetup();
  
  delay(100);
}

//=================================================================================
//           888                        888               
//           888                        888               
//           888                        888               
//   .d8888b 88888b.   .d88b.   .d8888b 888  888 .d8888b  
//  d88P"    888 "88b d8P  Y8b d88P"    888 .88P 88K      
//  888      888  888 88888888 888      888888K  "Y8888b. 
//  Y88b.    888  888 Y8b.     Y88b.    888 "88b      X88 
//   "Y8888P 888  888  "Y8888   "Y8888P 888  888  88888P'
//=================================================================================
elapsedMillis msecs;
/*
 * We need to check a number of functions which can result
 * in long-running transactions
 */
elapsedMicros checkMicros;
uint16_t colours[] = {ST7735_RED, CL(255,128,0), ST7735_YELLOW, ST7735_GREEN, ST7735_CYAN, ST7735_BLUE, ST7735_MAGENTA, ST7735_WHITE};

//------------------------------------------------------------------------
uint16_t nextColour(void)
{
  static int idx;
  uint16_t result = colours[idx];

  if (++idx >= COUNT_OF(colours))
    idx = 0;

  return result;    
}

//------------------------------------------------------------------------
volatile bool frameCompleted;
volatile int completedFrames, halfFrames;
volatile uint32_t startedAt, halfFrameAt, fullFrameAt;
void frameCompleteFn(void)
{
  if (tft.subFrameCount())
  {
    halfFrames++;
    halfFrameAt = micros();
  }
  else    
  {
    frameCompleted = true;
    completedFrames++;
    fullFrameAt = micros();
  }
}


void reportFrameTimes(void)
{
  Serial.printf("Callbacks: half @ %dus, full @ %dus\n", 
                halfFrameAt - startedAt, fullFrameAt - startedAt);
}

//------------------------------------------------------------------------
uint32_t check_(void)
{
  checkMicros = 0;

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_fillRect(void)
{
  checkMicros = 0;

  tft.fillRect(0,0,SQ,SQ,nextColour());

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_drawChar(void)
{
  tft.fillRect(SQ+96,0,96,SQ,nextColour());
  checkMicros = 0;

  tft.setFont();
  tft.setTextSize(8);
  tft.setTextColor(nextColour());
  tft.setCursor(SQ+96,0);
  tft.print("qb");

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_drawChar_bg(void)
{
  checkMicros = 0;

  tft.setFont();
  tft.setTextSize(8);
  tft.setTextColor(nextColour(),ST7735_BLACK);
  tft.setCursor(SQ,0);
  tft.print("t8");

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_drawFontChar(void)
{
  tft.fillRect(SQ+96,SQ,96,90,nextColour());
  checkMicros = 0;

  tft.setFont(Arial_60);
  tft.setTextColor(nextColour());
  tft.setCursor(SQ+96,SQ);
  tft.print("qb");

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_drawFontChar_bg(void)
{
  checkMicros = 0;

  tft.setFont(Arial_60);
  tft.setTextColor(nextColour(),0x38E7);
  tft.setCursor(SQ,SQ);
  tft.print(" t8 ");

  return checkMicros;
}
//------------------------------------------------------------------------
/* // Doesn't work, and isn't really supposed to!
uint32_t check_drawAAChar(void)
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
uint32_t check_drawAAChar_bg(void)
{
  checkMicros = 0;

  tft.setFont(OpenSans60);
  tft.setTextColor(nextColour(),0x38E7);
  tft.setCursor(SQ+96*2,0);
  tft.print(" t8 ");

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_writeRect(void)
{
  checkMicros = 0;
  static int which;

  uint16_t* wr = (which&16)?save1:save2; // only change every 16 calls
  tft.writeRect(0,SQ,SQ,SQ,wr);
  which++;// = !which;

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_writeSubImageRect(void)
{
  tft.fillRect(0,SQ*2,SQ,SQ,nextColour()); // don't count this
  
  checkMicros = 0;
  static int which;
  int off = 8;

  uint16_t* wr = (which&32)?save1:save2;
  tft.writeSubImageRect(off,SQ*2+off,SQ-off*2,SQ-off*2,
                        off, off, SQ, SQ,
                        wr);
  which++;// = !which;

  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_writeSubImageRectBytesReversed(void)
{
  tft.fillRect(0,SQ*3,SQ,SQ,nextColour()); // don't count this
  
  checkMicros = 0;
  static int which;
  int off = 8;

  uint16_t* wr = (which&8)?save1:save2;
  tft.writeSubImageRect(off,SQ*3+off,SQ-off*2,SQ-off*2,
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
uint32_t check_writeRect1BPP(void)
{  
  checkMicros = 0;
  nextPalette();
  tft.writeRect1BPP(SQ, SQ+96, 80, 80, &img1bpp80x80[0], palette + palIdx);
  // tft.writeRect1BPP(SQ, SQ+96, 16, 16, pict1bpp, palette + palIdx);
  return checkMicros;
}

//------------------------------------------------------------------------
extern uint8_t img2bpp80x80[];
uint32_t check_writeRect2BPP(void)
{  
  checkMicros = 0;
  nextPalette();
  tft.writeRect2BPP(SQ*2, SQ+96, 80, 80, &img2bpp80x80[0], palette + palIdx);
  //tft.writeRect2BPP(SQ*2, SQ+96, 32, 16, pict2bpp, palette + palIdx);  
  return checkMicros;
}

//------------------------------------------------------------------------
extern uint8_t img4bpp80x80[];
uint32_t check_writeRect4BPP(void)
{  
  checkMicros = 0;
  // tft.writeRectNBPP(SQ*3, SQ+96, 16, 16, 4, pict4bpp, palette);
  nextPalette();
  tft.writeRectNBPP(SQ*3, SQ+96, 80, 80, 4, &img4bpp80x80[0], palette + palIdx);
  return checkMicros;
}

//------------------------------------------------------------------------
uint32_t check_updateScreen(void)
{  
  checkMicros = 0;
  tft.updateScreen();
  return checkMicros;
}
//------------------------------------------------------------------------
int whichBox;
uint32_t check_updateClip(void)
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
      tft.setClipRect(0,0,480,40);
      break;

    case 3:
      tft.setClipRect(40,120,440,80);
      break;

    case 4:
      tft.setClipRect(0,40,40,280);
      break;

    case 5:
      tft.setClipRect(120,40,307,80);
      break;

    case 6:
      tft.setClipRect(260,200,67,83);
      break;
  
    case 7:
      tft.setClipRect(40,200,40,120);
      break;
  
    case 8:
      tft.setClipRect(80,240,180,17);
      break;
  }

  if (whichBox >= 0)
  {
    tft.updateScreenAsync(false,true,true);
  }
  whichBox++;
  return whichBox-1;
}

//========================================================================
elapsedMicros asyncTime;
void run_async_check(const char* name, uint32_t num)
{
  Serial.printf("Async check %s: %d;", name, num);
  tft.updateChangedAreasOnly(true);
  asyncTime = 0;
  tft.updateScreenAsync(false,true);
  tft.waitUpdateAsyncComplete(); // could do stuff here...
  Serial.printf(" took %dus\n",(int) asyncTime);
}

#if 9 == UPDATE_MODE || 10 == UPDATE_MODE
  #define RUN_CHECK(n)  run_async_check(#n,check_##n())
#else
  #define RUN_CHECK(n) Serial.printf("%9d: check " #n ": %d\n", millis(), check_##n())
#endif // 9 or 10 == UPDATE_MODE

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
uint32_t lastCheck;
bool asyncStarted;

void serialWait(void)
{
  while (!Serial.available())
    ;
  while (Serial.available())
  {
    Serial.read();
    delay(2);
  }
}


void loop() 
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

  //*
  if (millis() - lastCheck >= 100 // auto trigger
      && !asyncStarted
      ) 
  {
    lastCheck = millis();
  /*/
  if (Serial.available()) // manual trigger
  {
    while (Serial.available())
      Serial.read();
  //*/

    Serial.println("========================");
    RUN_CHECK(fillRect);
    RUN_CHECK(drawChar_bg);
    RUN_CHECK(drawChar);
    RUN_CHECK(drawFontChar_bg);
    RUN_CHECK(drawFontChar);
    RUN_CHECK(drawAAChar_bg);
    // RUN_CHECK(drawAAChar);
    RUN_CHECK(writeRect);
    RUN_CHECK(writeSubImageRect);
    RUN_CHECK(writeSubImageRectBytesReversed);
    RUN_CHECK(writeRect1BPP);
    RUN_CHECK(writeRect2BPP);
    RUN_CHECK(writeRect4BPP);

    whichBox = 0; // re-start clipped boxes in update mode 8
    completedFrames = 0; // and completed frames
    halfFrames = 0;
    startedAt = micros();
    
    switch (UPDATE_MODE)
    {
      default:
        break;

      case 1:
        RUN_CHECK(updateScreen);
        break;
        
      case 2:
      case 3:
      case 5:
        Serial.printf("Async start was %sOK; %d chunks/frame\n",
                       tft.updateScreenAsync(3 == UPDATE_MODE)?"":"not ",
                       tft.getMaxFrameCount()
                      );
        asyncTime = 0;
        asyncStarted=true;
        delay(10);
        break;

      case 6:
      case 7:
        Serial.printf("Async start was %sOK; %d chunks/frame\n",
            tft.updateScreenAsync(7 == UPDATE_MODE,true)?"":"not ",
            tft.getMaxFrameCount()
           );
        asyncTime = 0;
        asyncStarted=true;
        delay(10);
        break;

      case 8:
        RUN_CHECK(updateClip);
        asyncStarted = true;
        asyncTime = 0;
        msecs = 999999;
        break;

      case 11:
      case 12:
      case 13:
        // update whole screen to start with
        Serial.printf("Whole-screen async start was %sOK; %d chunks/frame\n",
                      tft.updateScreenAsync(false,true)?"":"not ",
                      tft.getMaxFrameCount()
                    );
        tft.waitUpdateAsyncComplete(); // wait for that to finish
        reportFrameTimes();
        delay(4);

        // now start a continuous update of part of the screen
        //tft.updateChangedAreasOnly(true);
        tft.setClipRect(SQ*3, SQ+96, 80, 80);
        Serial.printf("Async start was %sOK; %d chunks/frame\n",
                      tft.updateScreenAsync(true,true,true)?"":"not ",
                      tft.getMaxFrameCount()
                    );
        startedAt = micros();
        asyncStarted = true;
        asyncTime = 0;
        break;
          

#if  defined ST77XX_BLACK
      case 4:
        Serial.printf("Async start was %sOK; %d chunks/frame\n",
                      tft.updateScreenAsyncT4()?"":"not ",
                      tft.getMaxFrameCount()
                    );
        asyncTime = 0;
        asyncStarted=true;
        msecs = 999999;
        break;
#endif // defined ST77XX_BLACK
    }
    if (msecs != 999999)
      msecs = 0;
  }

  /*
  while (asyncStarted && tft.asyncUpdateActive())
  {
    uint16_t x0, y0, x1, y1;
    tft.getAddr(x0, y0, x1, y1);
    Serial.printf("%d, %d, %d, %d\n",x0, y0, x1, y1);
    delay(5);
  }
  */
  if (asyncStarted)
  {
    printDMAchannel(); // only happens once

    /*
     * The first async update initialises the DMA, but for
     * mode 8 we need the pre-emptible DMA library, and that
     * can cause issues with lowering the DMA interrupt priority
     * if e.g. SPI is on channel 0 and audio on channel 16
     * 
     * Hence, AFTER the first update is started, and once only,
     * we force the interrupt to low priority. Audio still seems
     * to work OK, though a better strategy is needed, really.
     */
#if defined(xDMA_DCHPRI_DPA)  // DMA has pre-emption capability
    static bool forceDone = false;
    if (!forceDone)
    {
      forceDone = true;
      tft.forceAsyncInterruptPriority(224); // lower the DMA interrupt priority
    }
#endif // defined(DMA_DCHPRI_DPA)    

    if (tft.asyncUpdateActive())
    {
      // how long to do foreground changes for...
      uint32_t timeLimit = 100'000; // ...in microseconds

      switch (UPDATE_MODE)
      {
        default: break;

        case 3:
        case 7:
          timeLimit = 250'000;
          // fallthrough
        case 11:
        case 12:
        case 13:
        {
          elapsedMicros t = 0;
          bool areaChanged = false;
          // uint32_t delayOnStop = random(60);
          static uint32_t delayOnStop = 0;
          if (++delayOnStop > 60)
            delayOnStop = 0;

          while (t < timeLimit) // for a while
          {
            // use callback to flag when frame is done
            if (frameCompleted)
            {
              frameCompleted = false;
              
              reportFrameTimes();
              startedAt = micros();
      
              if (areaChanged)
              {
                areaChanged = false;
                Serial.printf("Now %d chunks/frame\n", tft.getMaxFrameCount());
              }

              if (12 == UPDATE_MODE && 7 == completedFrames)
              {
                tft.setClipRect(SQ*2, SQ+96, 160, 80);
                tft.changeAsyncClipArea();
                areaChanged = true;
                Serial.println("Set clip rectangle and change async area");
              }

              if (13 == UPDATE_MODE)
              {
                switch (completedFrames)
                {
                  case 7:
                    tft.setClipRect(SQ*2, SQ+96, 160, 80);
                    Serial.println("Set clip rectangle");
                    tft.clearChangedArea();
                    break;

                  case 8:
                    tft.updateChangedAreasOnly(true);
                    tft.changeAsyncClipArea();
                    areaChanged = true;
                    Serial.println("Change async area");
                    break;
                }
              }

              RUN_CHECK(writeRect2BPP);
              RUN_CHECK(writeRect4BPP);
              tft.flushFramebufferCache(); // yup, it's needed!
            }
          }

          while (!frameCompleted)
            ;//delay(1); // a bit of time to allow last update to appear
          Serial.printf("Stop async update after %dus: ", delayOnStop);
          // digitalWriteFast(1,1); // could end async now, but...
          delayMicroseconds(delayOnStop); // ... wait a while
          t = 0;
          tft.endUpdateAsync();
          /*
          // Issues with stopping: see what the state is doing
          uint8_t st=0x80;
          while (st & 0x80)
          {
            st = tft.asyncState();
            Serial.printf("%02X\n",st);
            if (t<10'000)
              delayMicroseconds(500);
            else
            {
              digitalWriteFast(2,1);
              delay(250);              
              digitalWriteFast(2,0);
            }
          }
          /*/
          //digitalWriteFast(2,0);
          tft.waitUpdateAsyncComplete();
          //*/

          Serial.printf("took %dus, %d tries; %d frames, %d half-frames completed\n", 
                        (int) t, tft.getEndUpdateTries(),
                        completedFrames, halfFrames);
          tft.updateChangedAreasOnly(false); // just in case
          tft.setClipRect(); // ditto
          asyncStarted = false;
          delay(100);
        }
          break;
      }

#if defined ST77XX_BLACK
      if (msecs > 50)
      {
        msecs = 0;
#if defined(CLASSES_UNPROTECTED)
        Serial.printf("%dus: frames: %d; state %02X; CSR %08X; SADDR %08X\n", 
                    (int) asyncTime,
                    tft.getFrameCount(0), 
                    tft._dma_state,
                    tft._dma_data[0]._dmatx.TCD->CSR,
                    tft._dma_data[0]._dmatx.TCD->SADDR
                  );
#else                  
        Serial.printf("%dus: frames: %d\n", 
                    (int) asyncTime,
                    tft.getFrameCount(0)
                  );
#endif // defined(CLASSES_UNPROTECTED)                    
      }
#endif // defined ST77XX_BLACK    
    }
    else
    {
      // start another clipped area updating?
      if ((8 == UPDATE_MODE))
      {
        Serial.printf("...took %dus\n", (int) checkMicros);
        delay(50);
        RUN_CHECK(updateClip);  // do next area
        if (0 == whichBox)      // there isn't one...
        {
          tft.setClipRect(); // let screen writes use the whole buffer
          asyncStarted = false; // ...so stop
          delay(500);
        }
      }
      else
      {
        asyncStarted = false;
        if (3 != UPDATE_MODE // timer makes no sense for continuous update
         && 7 != UPDATE_MODE)
          Serial.printf("Async update took %dus\n",(uint32_t) asyncTime);
        if (7 != UPDATE_MODE)          
          delay(500);
      }
    }
  }
}
