#ifndef axBase_included
#define axBase_included
//----------------------------------------------------------------------

// when everything is up'n'running again:
// we could remove the constructor/destructor from all base classes,
// also, examine the virtual-ness of everything (what's needed, etc)

//----------

#include "hack_hack.h"

#include "axConfig.h"
#include "core/axDefines.h"
//#include "core/axDebug.h"
//#include "core/axAssert.h"
#include "core/axRect.h"
#include "core/axMalloc.h"
#include "core/axRand.h"
#include "core/axStdlib.h"
#include "core/axUtils.h"

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
    virtual axPlatform*   getPlatform(void)       { return NULL; }
    virtual axInterface*  getInterface(void)      { return NULL; }
    virtual axFormat*     getFormat(void)         { return NULL; }
    virtual axDescriptor* createDescriptor(void)  { return NULL; }
    virtual axInstance*   createInstance(void)    { return NULL; }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class axDescriptor
{
  public:
    axDescriptor(axBase* aBase) { /*trace("axDescriptor.constructor");*/ }
    virtual ~axDescriptor()     { /*trace("axDescriptor.destructor");*/ }
    //
    virtual bool hasEditor(void) { /*trace("hasEditor");*/ return false; }
};

//----------

class axInstance
{
  public:
    axInstance(axBase* aBase) { /*trace("axInstance.constructor");*/ }
    virtual ~axInstance()     { /*trace("axInstance.destructor");*/ }
    //
    virtual bool hasFlag(int aFlag) { return false; }
    //
    virtual void*   doOpenEditor(void* ptr) { return NULL; }
    virtual void    doCloseEditor(void)     { }
    virtual axRect  getEditorRect(void) { return axRect(0,0,100,100); }
};

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
    virtual void* createWindow(void* aParent, axRect aRect, int aFlags) { return NULL; }

};

//----------------------------------------------------------------------

// entrypoint:
// Vst:    ptr = audioMasterCallback, return AEffect
// Ladspa: ptr = ?, return = LADSPA_Descriptor
// Exe:    ptr = portaudio/rtaudio/libaam/.., return ?

class axFormat
{
  //private:
  //  int result;
  public:
    axFormat(axBase* aBase) { /*trace("axFormat.constructor");*/ }
    virtual ~axFormat()     { /*trace("axFormat.destructor");*/ }
    //virtual void* entrypoint(void* ptr) { result=0; return &result; }
    virtual char* getFormatName(void) { return (char*)""; }

};

//----------------------------------------------------------------------
// base implementation
//----------------------------------------------------------------------

  #define MAKESTRING2(s) #s
  #define MAKESTRING(s) MAKESTRING2(s)
  #define MAKE_NAME(name) MAKESTRING(name) "_window"

//char* winname = (char*)MAKE_NAME(name);

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
    //axDescriptor*   mDescriptor;
  public:
    axBaseImpl()
      {
        //trace("axBaseImpl.constructor");
        mPlatform     = new _PL(this);
        mInterface    = new _IF(this);
        //mDescriptor   = new _D(this);
        mFormat       = new _FO(this);
      }
    virtual ~axBaseImpl()
      {
        //trace("axBaseImpl.destructor");
        delete mPlatform;
        delete mInterface;
        //delete mDescriptor;
        delete mFormat;
      }
  //protected:
  public:
    virtual axPlatform*   getPlatform(void)       { return mPlatform; }
    virtual axInterface*  getInterface(void)      { return mInterface; }
    virtual axFormat*     getFormat(void)         { return mFormat; }
    virtual axDescriptor* createDescriptor(void)  { return new _D(this); } // you have to delete it yourself
    virtual axInstance*   createInstance(void)    { return new _I(this); } // --"--
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// singleton???

class axGlobalScope
{
  private:
    axBase* mBase;
  public:
    axGlobalScope()  { /*trace("axGlobalScope.constructor");*/ mBase=NULL; }
    ~axGlobalScope() { /*trace("axGlobalScope.destructor");*/ if (mBase) delete mBase; }
    inline void setBase(axBase* aBase) { mBase=aBase; }
};

//----------

// __thread?
static axGlobalScope gGlobalScope;

//----------------------------------------------------------------------

#include "base/axPlatform.h"
#include "base/axInterface.h"
#include "base/axFormat.h"

//----------------------------------------------------------------------
#endif


// win32: HWND   = pvoid
// linux: Window = unsigned long

//TODO: reparent (axWindowWin32/Linux), use void* as parent, not int (32/64 bit safety)
