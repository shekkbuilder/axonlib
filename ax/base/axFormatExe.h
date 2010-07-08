#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorExe : public axDescriptor
{
  public:
    axDescriptorExe(axBase* aBase) : axDescriptor(aBase) { trace("axDescriptorExe.constructor"); }
    virtual ~axDescriptorExe() { trace("axDescriptorExe.destructor"); }
};

typedef axDescriptorExe AX_DESCRIPTOR;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstanceExe : public axInstance
{
  private:
    int result;
  public:
    axInstanceExe(axBase* aBase) : axInstance(aBase) { trace("axInstanceExe.constructor"); }
    virtual ~axInstanceExe() { trace("axInstanceExe.destructor"); }

};

typedef axInstanceExe AX_INSTANCE;

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

class axFormatExe : public axFormat
{

  //friend int main(void);
  friend int main(int argc, char** argv);

  private:
    int result;

  //----------

  protected:
    axBase*       mBase;
    axDescriptor* mDescriptor;
    axInstance*   mInstance;

  protected:

    virtual void* entrypoint(void* ptr)
      {
        trace("*   axFormatExe.entrypoint   *");
        result = 0;
        return &result;
      }

  //----------

  public:

    axFormatExe(axBase* aBase) : axFormat(aBase)
      {
        trace("axFormatExe.constructor");
        mBase       = aBase;
        mDescriptor = mBase->createDescriptor();
        mInstance   = mBase->createInstance();
      }

    virtual ~axFormatExe()
      {
        trace("axFormatExe.destructor");
        delete mDescriptor;
        delete mInstance;
      }

};

//----------

typedef axFormatExe AX_FORMAT;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

// _PL = platform   win32, linux
// _IF = interface  win32, linux, nogui
// _FO = format     exe, vst, ladspa

#define AX_ENTRYPOINT(_PL,_IF,_FO,_D,_I)                                      \
                                                                              \
int main(int argc, char** argv)                                               \
{                                                                             \
  axBaseImpl<_PL,_IF,_FO,_D,_I>* base = new axBaseImpl<_PL,_IF,_FO,_D,_I>();  \
  gGlobalScope.setBase(base);                                                 \
  _FO* format = (_FO*)base->getFormat();                                      \
  int result = *(int*)format->entrypoint(NULL);                               \
  return result;                                                              \
}

//TODO:
//  entrypoint.ptr = parsed command line arguments?

//----------------------------------------------------------------------
#endif
