#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatform
{
  //private:
  //  int mFormat;
  public:
    axPlatform(int aFormat)
      {
        //mFormat = aFormat;
        //#ifndef AX_NOGUI
        //  XInitThreads();
        //#endif
      }
    virtual ~axPlatform() { }
    //inline int  getOs(void)     { return pf_Linux; }
    //inline int  getFormat(void) { return mFormat; }
};

//----------------------------------------------------------------------
#endif

