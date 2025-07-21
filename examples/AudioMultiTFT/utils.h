//
#if !defined(_UTILS_H_)
#define _UTILS_H_
extern uint16_t nextColour(bool inc = true);
extern void safeCSpins(void);

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
#endif // defined(_UTILS_H_)