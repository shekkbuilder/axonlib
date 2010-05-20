#ifndef axImage_included
#define axImage_included
//----------------------------------------------------------------------

class axImage
{
  public:
    virtual int getHandle(void) { return  0; }
    virtual int getPicture(void) { return 0; }
};

//----------------------------------------------------------------------
#endif

