#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  protected:
    axBase* mBase;
  private:    

  public:
    axPlatformLinux(axBase* aBase)// : axPlatform(aBase)
      {
        /*trace("axPlatformLinux.constructor");*/
        mBase = aBase;        
      }
    //virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
    virtual char* getPlatformName(void) { return (char*)"linux"; }    
};

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------

__externc void _ax_init(void) __constructor;
__externc void _ax_init(void)
{
  std::cout << "_ax_init()" << std::endl;
}

__externc void _ax_fini(void) __destructor;
__externc void _ax_fini(void)
{
  std::cout << "_ax_fini()" << std::endl;
}

//----------------------------------------------------------------------
#endif
