#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGDELETE)

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorExe : public axDescriptor
{
  public:
    axDescriptorExe(axBase* aBase) : axDescriptor(aBase) { /*trace("axDescriptorExe.constructor");*/ }
    virtual ~axDescriptorExe() { /*trace("axDescriptorExe.destructor");*/ }
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
    axInstanceExe(axBase* aBase) : axInstance(aBase) { /*trace("axInstanceExe.constructor");*/ }
    virtual ~axInstanceExe() { /*trace("axInstanceExe.destructor");*/ }

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
    //int           mFormatFlags;

  protected:

    virtual void* entrypoint(void* ptr)
      {
        //trace("++ axFormatExe.entrypoint   *");
        #ifndef AX_NOGUI
        //if (mFlags&if_HasEditor)
        //if (mDescriptor->hasEditor())
        {
          //trace("hasEditor");
          axWindow* win = (axWindow*)mInstance->doOpenEditor(ptr/*NULL*/);
          if (win)
          {
            win->show();
            win->eventLoop();
            win->hide();
          }
          mInstance->doCloseEditor();
        }
        //trace("-- axFormatExe.entrypoint   *");
        #endif
        result = 0;
        return &result;
      }

  //----------

  public:

    axFormatExe(axBase* aBase) : axFormat(aBase)
      {
        //trace("axFormatExe.constructor");
        mBase       = aBase;
        mDescriptor = mBase->getDescriptor();
        mInstance   = mBase->createInstance();
        //mFormatFlags = ff_None;
      }

    virtual ~axFormatExe()
      {
        //trace("axFormatExe.destructor");
        //mInstance->doCloseEditor();
        delete mDescriptor;
        delete mInstance;
      }

    //----------

    virtual char* getFormatName(void)
      {
        return (char*)"exe";
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
