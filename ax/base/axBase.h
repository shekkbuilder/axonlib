#ifndef axBase_included
#define axBase_included
//----------------------------------------------------------------------

// we could remove the constructor/destructor from all base classes,
// also, examine the virtual-ness of everything..
// and third, descriptor/instance?

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axAssert.h"
//#include "core/axRect.h"
//#include "par/axParameter.h"

//----------------------------------------------------------------------

class axPlatform;
class axInterface;
class axFormat;
//class axDescriptor;
class axInstance;

//----------------------------------------------------------------------

class axBase
{
  public:
    axBase() {}
    virtual ~axBase() {}
    virtual axPlatform*   getPlatform(void)       { return NULL; }
    virtual axInterface*  getInterface(void)      { return NULL; }
    virtual axFormat*     getFormat(void)         { return NULL; }
};

//----------------------------------------------------------------------
//
// cross-*
//
//----------------------------------------------------------------------

// Win32: DllMain, HINSTANCE
// Linux:

class axPlatform
{
  public:
    axPlatform(axBase* aBase) { trace("  axPlatform.constructor"); }
    virtual ~axPlatform()     { trace("  axPlatform.destructor"); }
};

//----------------------------------------------------------------------

// Linux: Display*
// Win32: register class

class axInterface
{
  public:
    axInterface(axBase* aBase)  { trace("  axInterface.constructor"); }
    virtual ~axInterface()      { trace("  axInterface.destructor"); }
    virtual void* createWindow(axInstance* aInstance, void* parent) { return NULL; }
};

//----------------------------------------------------------------------

// entrypoint:
// Vst:    ptr = audioMasterCallback, return AEffect
// Ladspa: ptr = ?, return = LADSPA_Descriptor
// Exe:    ptr = portaudio/rtaudio/libaam/.., return ?

class axFormat
{
  private:
    int result;
  public:
    axFormat(axBase* aBase) { trace("  axFormat.constructor"); }
    virtual ~axFormat()     { trace("  axFormat.destructor"); }
    virtual void*         entrypoint(void* ptr)   { result=0; return &result; }
    //virtual axDescriptor* createDescriptor(void)  { return NULL; }
    virtual axInstance*   createInstance(void)    { return NULL; }

};

//----------------------------------------------------------------------

//class axDescriptor
//{
//  public:
//    axDescriptor(axBase* aBase) { trace("  axDescriptor.constructor"); }
//    virtual ~axDescriptor()     { trace("  axDescriptor.destructor"); }
//};

//----------

class axInstance
{
  public:
    axInstance(axBase* aBase) { trace("  axInstance.constructor"); }
    virtual ~axInstance()     { trace("  axInstance.destructor"); }
    //virtual char* getName(void) { return (char*)"axInstance"; }
};

//----------------------------------------------------------------------
//
// base implementation
//
//----------------------------------------------------------------------

/*

PL - Platform
IF - Interface
FO - Format

D  - Descriptor
I  - Instance

*/

template<class _PL, class _IF, class _FO>
class axBaseImpl : public axBase
{
  private:
    axPlatform*   mPlatform;
    axInterface*  mInterface;
    axFormat*     mFormat;
  public:
    axBaseImpl()
      {
        trace("  axBaseImpl.constructor");
        mPlatform   = new _PL(this);
        mInterface  = new _IF(this);
        mFormat     = new _FO(this);
      }
    virtual ~axBaseImpl()
      {
        trace("  axBaseImpl.destructor");
        delete mPlatform;
        delete mInterface;
        delete mFormat;
      }
  //protected:
  public:
    virtual axPlatform*   getPlatform(void)   { return mPlatform; }
    virtual axInterface*  getInterface(void)  { return mInterface; }
    virtual axFormat*     getFormat(void)     { return mFormat; }
};

//----------------------------------------------------------------------

// singleton???

class axGlobalScope
{
  private:
    axBase* mBase;
  public:
    axGlobalScope()  { trace("  axGlobalScope.constructor"); mBase=NULL; }
    ~axGlobalScope() { trace("  axGlobalScope.destructor");  if (mBase) delete mBase; }
    inline void setBase(axBase* aBase) { mBase=aBase; }
};

//----------

// __thread?
static axGlobalScope g_GlobalScope;

//----------------------------------------------------------------------

#include "base/axPlatform.h"
#include "base/axInterface.h"
#include "base/axFormat.h"

#define AX_MAIN(format) AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,format)

//----------------------------------------------------------------------
#endif
