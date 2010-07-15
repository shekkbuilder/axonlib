#ifndef axBase_included
#define axBase_included
//----------------------------------------------------------------------
/*

- axGlobalScope
  - base = NULL
  - ptr = NULL
- DllMain [win32]
  - gWinInstance
- AX_ENTRYPOINT
  - create base <template classes>
    - create platform
      - GetModuleFileName/dladdr (for axGetBasePath)
      - create debug
        - global.setPtr(debug)
    - create interface
    - create descriptor
    - create format
  - global.setBase(base)
  - format.entrypoiunt
    - create instance [vst]

*/

#include "hack_hack.h"

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axRect.h"

#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axRand.h"
#include "core/axStdlib.h"
#include "core/axUtils.h"

#include "par/axParameter.h"
//^ moved some of the includes bellow the global singleton

//----------------------------------------------------------------------


//----------------------------------------------------------------------

class axPlatform;
class axInterface;
class axFormat;
class axDescriptor;
class axInstance;

//----------------------------------------------------------------------

class axBase
{
  public:
    axBase() {}
    virtual ~axBase() {}
    virtual axPlatform*   getPlatform(void)     { return NULL; }
    virtual axInterface*  getInterface(void)    { return NULL; }
    virtual axFormat*     getFormat(void)       { return NULL; }
    virtual axDescriptor* getDescriptor(void)   { return NULL; }
    virtual axInstance*   createInstance(void)  { return NULL; }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "base/axDescriptor.h"
#include "base/axInstance.h"

//----------------------------------------------------------------------
// cross-*
//----------------------------------------------------------------------

// Win32: DllMain, HINSTANCE
// Linux:

class axPlatform
{
  public:
    axPlatform(axBase* aBase) { /*trace("axPlatform.constructor");*/ }
    virtual ~axPlatform()     { /*trace("axPlatform.destructor");*/ }
    virtual void* getHandle(void) { return NULL; }  // win32: hinstance
    virtual char* getPlatformName(void) { return (char*)""; }
    virtual char* getPath(void) { return (char*)""; }
};

//----------------------------------------------------------------------

// Linux: Display*
// Win32: register class

class axInterface
{
  public:
    axInterface(axBase* aBase)  { /*trace("axInterface.constructor");*/ }
    virtual ~axInterface()      { /*trace("axInterface.destructor");*/ }
    virtual void* getHandle(void) { return NULL; }    // linux: display*
    virtual char* getName(void) { return (char*)""; } // win32: window class name
    virtual void* createEditor(void* aParent, axRect aRect, int aFlags) { return NULL; }
};

//----------------------------------------------------------------------

// entrypoint:
// Vst:    ptr = audioMasterCallback, return AEffect
// Ladspa: ptr = ?, return = LADSPA_Descriptor
// Exe:    ptr = portaudio/rtaudio/libaam/.., return ?

class axFormat
{
  public:
    axFormat(axBase* aBase) { /*trace("axFormat.constructor");*/ }
    virtual ~axFormat()     { /*trace("axFormat.destructor");*/ }
    virtual void* entrypoint(void* ptr) { return NULL; }
    virtual char* getFormatName(void) { return (char*)""; }
    virtual void* getHostPtr(void) { return NULL; } // vst: audioMasterCallback
    virtual void* getUserPtr(void) { return NULL; } // vst: aeffect
};

//----------------------------------------------------------------------
// base implementation
//----------------------------------------------------------------------

// #############################################################################
// helper function which is never optimized
// a.k.a. " a test with the ugliest hack ever !!!"
// #############################################################################

inline axInstance* __instance_no_opt(axInstance* aMember)
  __attribute__ ((optimize(0)))  // <- specify a stable optimization level
  ;

inline axInstance* __instance_no_opt(axInstance* aMember)
{
  return aMember;
}
// #############################################################################

//  PL - Platform
//  IF - Interface
//  FO - Format
//
//  D  - Descriptor
//  I  - Instance

template<class _PL, class _IF, class _FO, class _D, class _I>
class axBaseImpl : public axBase
{
  private:
    axPlatform*     mPlatform;
    axInterface*    mInterface;
    axFormat*       mFormat;
    axDescriptor*   mDescriptor;
  public:
    axBaseImpl()
      {
        //trace("axBaseImpl.constructor");
        mPlatform     = new _PL(this);
        mInterface    = new _IF(this);
        mDescriptor   = new _D(this);
        mFormat       = new _FO(this);
      }
    virtual ~axBaseImpl()
      {
        //trace("axBaseImpl.destructor");
        delete mFormat;
        delete mDescriptor;
        delete mInterface;
        delete mPlatform;
      }
  //protected:
  public:
    virtual axPlatform*   getPlatform(void)     { return mPlatform; }
    virtual axInterface*  getInterface(void)    { return mInterface; }
    virtual axFormat*     getFormat(void)       { return mFormat; }
    virtual axDescriptor* getDescriptor(void)   { return mDescriptor; }

    virtual axInstance*   createInstance(void)
    {
      axInstance* aInstance = new _I(this);
      return aInstance;
      //return __instance_no_opt(aInstance);
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// singleton???
class axGlobalScope
{
  private:
    axBase*     mBase;
    void*       mPtr;
  public:
    axGlobalScope()  { mBase=NULL; mPtr=NULL; }
    ~axGlobalScope() { if (mBase) delete mBase; }
    inline void     setBase(axBase* aBase)
    {
      mBase=aBase;
      #if defined AX_DEBUG && defined AX_DEBUG_LOG
        axCout.setup();
      #endif
    }
    inline void     setPtr(void* aPtr)      { mPtr=aPtr; }
    inline axBase*  getBase(void)           { return mBase; }
    inline void*    getPtr(void)            { return mPtr; }
};

//----------

// __thread?
// __thread works only for primitive data types like 'int' or 'void*'
static axGlobalScope gGlobalScope;

//----------------------------------------------------------------------

// after gGlobalScope is defined we can include axDebug, which will need it
//

//#include "core/axMalloc.h"
//#include "core/axDebug.h"
//#include "core/axAssert.h"
//#include "core/axRand.h"
//#include "core/axStdlib.h"
//#include "core/axUtils.h"

#include "base/axPlatform.h"
#include "base/axInterface.h"
#include "base/axFormat.h"

//----------------------------------------------------------------------
#endif


// win32: HWND   = pvoid
// linux: Window = unsigned long

//TODO: reparent (axWindowWin32/Linux)
//      use void* as parent, not int (32/64 bit safety)
