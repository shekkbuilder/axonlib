#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#include "axPlatform.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptor
{
  public:
    axDescriptor(axPlatform* aPlatform) { }
    virtual ~axDescriptor() { }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstance
{
  public:
    axInstance(axDescriptor* aDescriptor) { }
    virtual ~axInstance() { }
    virtual int main(int argc, char** argv) { return 0; }
};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#include "axInterface.h"

template<class _D, class _I, class _In, class _P>
class axFormat
{
  private:
    _P*   mPlatform;
    _D*   mDescriptor;
    _I*   mInstance;
    _In*  mInterface;
  public:
    axFormat()
      {
        mPlatform   = new _P(ft_Exe);
        mDescriptor = new _D(mPlatform);
        mInstance   = new _I(mDescriptor);
        mInterface  = new _In(mInstance);
      }
    virtual ~axFormat()
      {
        delete mInterface;
        delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }
  public:
    inline axDescriptor* getDescriptor(void)  { return mDescriptor; }
    inline axInstance*   getInstance(void)    { return mInstance; }
    inline axInterface*  getInterface(void)   { return mInterface; }
    inline axPlatform*   getPlatform(void)    { return mPlatform; }
};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,axEditor,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                         \
                                                                                        \
int main(int argc, char** argv)                                                         \
{                                                                                       \
  axFormat<_desc,_inst,_iface,_plat>* exe =  new axFormat<_desc,_inst,_iface,_plat>();  \
  _inst* instance = (_inst*)exe->getInstance();                                         \
  int result = instance->main(argc,argv);                                               \
  delete exe;                                                                           \
  return result;                                                                        \
}

//----------------------------------------------------------------------
#endif
