#ifndef axImage_included
#define axImage_included
//----------------------------------------------------------------------

/*
  'ties' together a axBitmap and axSurface,
  so they both can be used in axCanvas's drawImage()
*/

class axImage
{
  public:
    virtual int getHandle(void) { return 0; }
};

//----------------------------------------------------------------------
#endif

