#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

// not really used for exe's, but, perhaps later..
class axDescriptor
{
  public:
    axDescriptor(axFormat* aFormat) { }
    virtual ~axDescriptor() { }
};

//----------------------------------------------------------------------

class axInstance
{
  public:
    axInstance(axFormat* aFormat) { }
    virtual ~axInstance() { }
    virtual int main(int argc, char** argv) { return 0; }
};

//----------------------------------------------------------------------
//
// format implementation
//
//----------------------------------------------------------------------

// thought: should this have the static callback methods for vst & ladspa?

// the exe file, abstracted out into 4 parts:
// system, info, process, gui

template<class _D,class _I,class _In,class _P>
class axFormatImpl : public axFormat
{
  private:
    /* _P*  */ axPlatform*   mPlatform;
    /* _D*  */ axDescriptor* mDescriptor;
    /* _I*  */ axInstance*   mInstance;
    /* _In* */ axInterface*  mInterface;

  public:
    axFormatImpl() : axFormat()
      {
        printf("axFormatImpl: exe\n");
        // for exe's create everything
        // vst will be similar
        // ladspa will create instance later (getInstance)
        mPlatform   = new _P(this);
        mDescriptor = new _D(this);
        mInstance   = new _I(this);
        mInterface  = new _In(this);
        // audio?
      }
    virtual ~axFormatImpl()
      {
        delete mPlatform;
        delete mDescriptor;
        delete mInstance;
        delete mInterface;
      }
  //protected: //TODO: friend func..
  public:
    // make them available to other classes that know about axFormat:
    // platform,interface,descriptor/instance
    virtual axFormat*     getFormat(void)     { return this; }
    virtual axPlatform*   getPlatform(void)   { return mPlatform; }
    virtual axDescriptor* getDescriptor(void) { return mDescriptor; }
    virtual axInstance*   getInstance(void)   { return mInstance; }
    virtual axInterface*  getInterface(void)  { return mInterface; }
};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                                 \
                                                                                                \
int main(int argc, char** argv)                                                                 \
{                                                                                               \
  axFormatImpl<_desc,_inst,_iface,_plat>* exe =  new axFormatImpl<_desc,_inst,_iface,_plat>();  \
  _inst* instance = (_inst*)exe->getInstance();                                                 \
  int result = instance->main(argc,argv);                                                       \
  delete exe;                                                                                   \
  return result;                                                                                \
}

//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,axInterface,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------
#endif
