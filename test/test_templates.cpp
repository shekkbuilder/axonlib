#define AX_NOGUI
#define AX_LINUX

#include <stdio.h> // printf
#include "temporary/axFormat.h"

//----------

//class myDescriptor : public axDescriptor
//{
//  public:
//    myDescriptor(axFormat* aFormat) : axDescriptor(aFormat) { printf("myDescriptor\n"); }
//};

//----------

class myInstance : public axInstance
{
  public:
    myInstance(axFormat* aFormat) : axInstance(aFormat) { printf("myInstance\n"); }
    int main(int argc,char** argv) { printf("myInstance.main\n"); return 0; }
};

//----------

//AX_MAIN(DEFAULT_DESCRIPTOR,myInstance)
//AX_ENTRYPOINT(myDescriptor,myInstance,axInterface,axPlatform)

AX_ENTRYPOINT( DEFAULT_DESCRIPTOR,
               myInstance,
               DEFAULT_INTERFACE,
               DEFAULT_PLATFORM )

//----------------------------------------------------------------------






























#if 0

// we might add audio here too (to replace axContext)

//#define AX_FORMAT_VST
#define AX_FORMAT_LADSPA

#include <stdio.h> // printf

class axFormat;
class axPlatform;
class axDescriptor;
class axInstance;
class axInterface;

//----------------------------------------------------------------------

// these two would be in other .h files, and selectively inbcluded
// depending on platform (AXLINUX, AX_WIN32)
// see axPlatform.h

// axPlatform.h + axPlatformLinux.h, axPlatformWin32.h

// win32: hinstance

class axPlatform
{
  public:
    axPlatform(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"no platform specificed"; }
};

// axInterface.h + axInterfaceNone.h, axInterfaceLinux.h, axInterfaceWin32.h,

// linux: XInitThreads
// win32: register/unregister window class

class axInterface
{
  public:
    axInterface(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"no interface specificed"; }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// these would also be in separate .h files, and included selectively by
// using AX_FORMAT_VST, AX_FORMAT_LADSPA, ...

#ifdef AX_FORMAT_VST
// axFormatVst.h

// describe/setup
// num ins/outs/params + names

class axDescriptor
{
  public:
    axDescriptor(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"vst descriptor"; }
};

// precess audio, handle midi/params

class axInstance
{
  public:
    axInstance(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"vst instance"; }
};

#endif

//----------

#ifdef AX_FORMAT_LADSPA
// axFormatLadspa.h

class axDescriptor
{
  public:
    axDescriptor(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"ladspa descriptor"; }
};

class axInstance
{
  public:
    axInstance(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"ladspa implementation"; }
};

#endif

//----------------------------------------------------------------------
//
// format implementation
//
//----------------------------------------------------------------------
// axFormat.h

// this is passed around in the constructor as an argument, and gives
// the other classes access to the low level 'root'
// some of the functions is not valid until all constructors have
// been properly run. depending on the order of creation

class axFormat
{
  public:
    axFormat() {}
    virtual axFormat*     getFormat(void)     { return this; }
    virtual axPlatform*   getPlatform(void)   { return NULL; }
    virtual axDescriptor* getDescriptor(void) { return NULL; }
    virtual axInstance*   getInstance(void)   { return NULL; }
    virtual axInterface*  getInterface(void)  { return NULL; }
};

//----------

// _D  = Descriptor class
// _I  = Instance
// _P  = Platform
// _In = Interface

// prepare (and make available) platform specific things
// and get the ball rolling for the specific format type

template<class _D,class _I,class _P,class _In>
class axFormatImpl : public axFormat
{
  private:
    axPlatform*   mPlatform;
    axDescriptor* mDescriptor;
    axInstance*   mInstance;
    axInterface*  mInterface;
  public:
    axFormatImpl() : axFormat()
      {
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
// for example...
//----------------------------------------------------------------------
// main.cpp

//these two would be our plugin

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axFormat* aFormat) : axDescriptor(aFormat) {}
    virtual char* getName(void) { return (char*)"my descriptor"; }
};

class myInstance : public axInstance
{
  private:
    float testfloat;
  public:
    myInstance(axFormat* aFormat) : axInstance(aFormat)
      {
        printf("  platform    = '%s'\n",aFormat->getPlatform()->getName());
        printf("  descriptor  = '%s'\n",aFormat->getDescriptor()->getName());
      }
    virtual char* getName(void) { return (char*)"my instance"; }
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// axFormatExe.h, axFormatLadspa, axFormatVst.h

// AX_ENTRYPOINT, using main() here, to simplify example..

int main(void)
{

  //axFormatImpl<axDescriptor,axInstance,axPlatform,axInterface>* exe
  //  = new axFormatImpl<axDescriptor,axInstance,axPlatform,axInterface>();

  axFormatImpl<myDescriptor,myInstance,axPlatform,axInterface>*
    exe = new axFormatImpl<myDescriptor,myInstance,axPlatform,axInterface>();

  // now our plugin/exe should be aive...

//    axFormat* format = exe->getFormat();
//    printf("  platform    = '%s'\n",format->getPlatform()->getName());
//    printf("  descriptor  = '%s'\n",format->getDescriptor()->getName());
//    printf("  instance    = '%s'\n",format->getInstance()->getName());
//    printf("  interface   = '%s'\n",format->getInterface()->getName());

  delete exe;
  return 0;
}

/*

- axFormat
connects everything,
sub-classed by axFormatImpl differently for each format: exe, vst, ladspa

you can use your own sub-classes of these four classes:
- axPlatform (linux,win32)
- axInterface (linux,win32,nogui)
- axDescriptor
- axInstance
but usually only the last two..

- axFormatImpl
created when the plugin loads (in AX_ENTRYPOINT)
initiates everything..

*/

#endif // 0
