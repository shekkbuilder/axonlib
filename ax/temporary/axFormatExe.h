#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

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
// main
//
//----------------------------------------------------------------------

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
        mPlatform   = new _P(this);
        mDescriptor = new _D(this);
        mInstance   = new _I(this);
        mInterface  = new _In(this);
      }
    virtual ~axFormatImpl()
      {
        delete mPlatform;
        delete mDescriptor;
        delete mInstance;
        delete mInterface;
      }
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

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,axInterface,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

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
#endif
