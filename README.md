This library supports ST7735 and ST7789 with and without a CS pin, such as https://www.amazon.com/gp/product/B07P9X3L7M/?pldnSite=1 which is a ST7789 240x240 display.

To initialize an instance of the display the user has a choice of constructors:
```c++
// This Teensy3 and 4 native optimized and extended version
// requires specific pins. 
// If you use the short version of the constructor and the DC
// pin is hardware CS pin, then it will be slower.

#define TFT_MISO  12
#define TFT_MOSI  11  //a12
#define TFT_SCK   13  //a13
#define TFT_DC     9 
#define TFT_CS    10  
#define TFT_RST    8

// Note the above pins are for the SPI object.  For those Teensy boards which have
// more than one SPI object, such as T3.5, T3.6, T4 which have at SPI1 and SPI2
// LC with SPI1, look at the cards that come with the teensy or the web page
// https://www.pjrc.com/teensy/pinout.html to select the appropriate IO pins.

#include <ST7735_t3.h> // Hardware-specific library
#include <ST7789_t3.h> // Hardware-specific library
#include <ST7796_t3.h> // Hardware-specific library
#include <SPI.h>

// Option 1: use any pins but a little slower
// Note: code will detect if specified pins are the hardware SPI pins
//       and will use hardware SPI if appropriate
// For 1.44" and 1.8" TFT with ST7735 use
ST7789_t3 tft = ST7789_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// For 1.54" or other TFT with ST7789, This has worked with some ST7789
// displays without CS pins, for those you can pass in -1 or 0xff for CS
// More notes by the tft.init call
//ST7789_t3 tft = ST7789_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
// For 1.44" and 1.8" TFT with ST7735 use
//ST7735_t3 tft = ST7735_t3(cs, dc, rst);

// For 1.54" TFT with ST7789
//ST7789_t3 tft = ST7789_t3(TFT_CS,  TFT_DC, TFT_RST);

// For 3.5" or 4.0" TFT with ST7796
//ST7796_t3 tft = ST7796_t3(TFT_CS, TFT_DC, TFT_RST);
```

NOTE: If the Teensy has more than one SPI buss, and the selected IO pins are _all_ on a different SPI buss then that buss will be used. (i.e. you can use SPI1 or SPI2).  With this, on a board such as a T4 or T3.5 or T3.6 you can potentially have three displays all on different SPI busses and using the Async updates you can have all three of them updating their display at the same time. 

For the case of a ST7789 the CS pin would be identified with a -1.  This constructor is used for those cases with the display on SPI and no CS pin:

```c++
//ST7789_t3 tft = ST7789_t3(TFT_CS,  TFT_DC, TFT_RST);
```

The library supports most if not all display resolutions:

```c++ 
// Use this initializer if you're using a 1.8" TFT 128x160 displays
//tft.initR(INITR_BLACKTAB);

// Or use this initializer (uncomment) if you're using a 1.44" TFT (128x128)
//tft.initR(INITR_144GREENTAB);

// Or use this initializer (uncomment) if you're using a .96" TFT(160x80)
//tft.initR(INITR_MINI160x80);

// Or use this initializer (uncomment) for Some 1.44" displays use different memory offsets
// Try it if yours is not working properly
// May need to tweek the offsets
//tft.setRowColStart(32,0);

// Or use this initializer (uncomment) if you're using a 1.54" 240x240 TFT
//tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels

// OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
tft.init(240, 320);           // Init ST7789 320x240

// OR use this initializer (uncomment) if using a 240x240 clone 
// that does not have a CS pin2.0" 320x240 TFT:
//tft.init(240, 240, SPI_MODE2);           // Init ST7789 240x240 no CS
```


Frame Buffer
------------

The Teensy 4.x, 3.6 and 3.5 have a lot more memory than previous Teensy processors, so on these boards, we borrowed some ideas from the ILI9341_t3DMA library and added code to be able to use a logical Frame Buffer.  

Since the smaller ST7735 and maybe ST7789 displays have fewer pixels, you can on some of them enable a frame buffer on a T3.2 as well. I believe in this case I did add support for Async updates as well. 

To enable this we added a couple of APIs 

```c++
uint8_t useFrameBuffer(bool b) - if b true it will allocate memory and start using
void	freeFrameBuffer(void) - Will free up the memory that was used.
void	updateScreen(void); - Will update the screen with all of your updates...
void	setFrameBuffer(uint16_t *frame_buffer); - Now have the ability allocate the frame buffer and pass it in, to avoid use of malloc
void updateChangedAreasOnly(bool b) - if b true only changed areas get updated on-screen
```

Asynchronous Update support (Frame buffer)
------------------------

The code now has support to use DMA for asynchronous updates of the screen.  You can choose to do the updates once or in continuous mode.  Note: I mainly use the oneshot, as we prefer more control on when the screen updates which helps to minimize things like flashing and tearing. 
Some of the New methods for this include: 

```c++
bool	updateScreenAsync(bool update_cont = false); // Starts an update either one shot or continuous
void	waitUpdateAsyncComplete(void);  // Wait for any active update to complete
void	endUpdateAsync();			 // Turn off the continuous mode.
boolean	asyncUpdateActive(void)      // Lets you know if an async operation is still active
```

Additional APIs
---------------
In addition, this library now has some of the APIs and functionality that has been requested in a pull request.  In particular it now supports the ability to set a clipping rectangle, as well as setting an origin that is used with the drawing primitives.   These new APIs include:

```c++
void setOrigin(int16_t x = 0, int16_t y = 0); 
void getOrigin(int16_t* x, int16_t* y);
void setClipRect(int16_t x1, int16_t y1, int16_t w, int16_t h); 
void setClipRect();
```

It also incorporates functionality from the TFT_ILI9341_ESP, https://github.com/Bodmer/TFT_ILI9341_ESP, for additional functions:

```c++
int16_t  drawNumber(long long_num,int poX, int poY);
int16_t  drawFloat(float floatNumber,int decimal,int poX, int poY);   
int16_t drawString(const String& string, int poX, int poY);
int16_t drawString1(char string[], int16_t len, int poX, int poY);
void setTextDatum(uint8_t datum);
```

In addition, scrolling text has been added using appropriate function from, https://github.com/vitormhenrique/ILI9341_t3:

```c++
void enableScroll(void);
void resetScrollBackgroundColor(uint16_t color);
void setScrollTextArea(int16_t x, int16_t y, int16_t w, int16_t h);
void setScrollBackgroundColor(uint16_t color);
void scrollTextArea(uint8_t scrollSize);
void resetScrollBackgroundColor(uint16_t color);
```
	
Font Support
------------
This library tries to support three different font types.  This includes the original font support that is in the ILI9341_t3 library, which is 
built in system font, as well as the new font format. 

In addition, we added support to use the Adafruit GFX fonts as well. This includes the ability to output the text in Opaque mode. 

Discussion regarding this optimized version:
==========================

https://forum.pjrc.com/threads/57015-ST7789_t3-(part-of-ST7735-library)-support-for-displays-without-CS-pin?highlight=st7735


Mid-transaction breaks (Teensy 4.x only) 
=========
A number of the drawing primitives take a signifcant time to execute, particularly at conservative SPI bus speed (e.g. 16MHz). Because claiming the SPI bus can interfere with other system activity, this driver has been extended to allow for "mid-transaction breaks", whereby the SPI transaction in progress is ended, `yield()` may be called, and the transaction resumed. This will allow any pending interrupt-driven access to the SPI bus to be executed.

Two API calls control this:
```c++
void setMaxTransaction(uint32_t us); // approximate maximum time between breaks
void enableYieldInMidTransaction(bool en); // true to call yield() in the break
```

The maximum transaction time is only approximate, because checks to see if the time has expired only occur at points when it is straightforward to suspend and resume the drawing. For example, with large character drawing, this is done after every character line. 

The default is to break every 1000us, but _not_ to `yield()`.

Extended asynchronous updates (Teensy 4.x only)
=====
Following on from the above, it is apparent that asynchronous updates will also take a long time to execute, especially for large displays. Further / updated API calls are provided to mitigate this:
```c++
void setMaxAsyncLines(int lines); // max lines to update between interrupts
void setAsyncInterruptPriority(int prio); // set interrupt priority
void forceAsyncInterruptPriority(int prio); // force-set interrupt priority
// call to trigger async screen update
bool  updateScreenAsync(bool update_cont = false, 	  // continuous updates
						bool interrupt_every = false, // interrupt after every set of lines
						bool use_clip_rect = false);  // only update part of the display

uint8_t useIntermediateBuffer(size_t s); // allocate intermediate buffer
uint8_t useIntermediateBuffer(void* m, size_t s) // use provided intermediate buffer
void  freeIntermediateBuffer(void);  // call to release the buffer							

bool changeAsyncClipArea(void); // notify continuous clipped updates of area change
void clearChangedArea(void);    // reset changed area to "nothing"
```
By setting the update to interrupt every few lines of output (`interrupt_every=true`), a mid-transaction break can occur if needed (see previous section). Note that in this case the break occurs inside an interrupt service routine, so `yield()` will _not_ be called, even if enabled.

To update only part of the display (`use_clip_rect=true`), _either_ set the desired area using `setClipRect()`, _or_ set `updateChangedAreasOnly(true)`; if you do both, then the latter limits will be used. In this case an intermediate buffer is _required_. This need not be very large: at a SPI speed of 16MHz it takes 1us to write one pixel (16 bits), so a 2kB buffer would take just over 1ms to write, which is a reasonable polling interval for the mid-transaction breaks.

When using this option, the area updated remains fixed until you call `changeAsyncClipArea()`: a new area will then be used _after_ the current async update completes. Care is needed if you have set `updateChangedAreasOnly(true)`, because the area boundaries are invalid if you haven't changed anything, and you may have a "stale" area and thus cause bigger updates than desired. The best sequence is as follows:
```c++
	tft.clearChangedArea(); // ensures area boundaries are reset (but invalid)
	// change something, e.g. write text or draw graphics; sets new area boundaries
	tft.changeAsyncClipArea(); // changes async updates to use the new area
```

Points to note:

* with asynchronous updates in progress, a mid-transaction break can _only_ take effect if the pending interrupt has a _higher_ priority than the async interrupt (_lower_ priority number)
* some use cases will only work with a revised `DMAChannel` driver which is not part of Teensyduino up to at least 1.59
* with the revised driver, it is possible that attempts to set the interrupt priority will fail unless `forceAsyncInterruptPriority()` is used

For example, the audio library uses an interrupt of priority 208; the async priority _must_ therefore be set to 224 or 240 to allow audio playback from SPI SD to work alongside async screen updates.

### Links ###
[Start of mid-transaction break discussion in existing ST7796 thread](https://forum.pjrc.com/index.php?threads/st7796-teensyduino-support.76510/post-358123)<br>
[Revised DMAChannel thread](https://forum.pjrc.com/index.php?threads/issues-with-dmachannels.76608/)


Adafruit library info
=======================

As this code is based on work by Adafruit, their original information is included below:

------------------------------------------

This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618
 
Check out the links above for our tutorials and wiring diagrams.
These displays use SPI to communicate, 4 or 5 pins are required
to interface (RST is optional).
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_ST7735. Check that the Adafruit_ST7735 folder contains Adafruit_ST7735.cpp and Adafruit_ST7735.

Place the Adafruit_ST7735 library folder your <arduinosketchfolder>/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE

Also requires the Adafruit_GFX library for Arduino.