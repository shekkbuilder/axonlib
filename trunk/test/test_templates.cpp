//#define AX_FORMAT_VST
#define AX_FORMAT_LADSPA

#include <stdio.h>

class axFormat;
class axPlatform;
class axDescriptor;
class axInstance;
class axInterface;

//----------------------------------------------------------------------
// these two would be in other .h files, and selectively inbcluded
// depending on platform (AXLINUX, AX_WIN32)

class axPlatform
{
  public:
    axPlatform(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"no platform specificed"; }
};

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

class axDescriptor
{
  public:
    axDescriptor(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"vst descriptor"; }
};

class axInstance
{
  public:
    axInstance(axFormat* aFormat) {}
    virtual char* getName(void) { return (char*)"vst instance"; }
};

#endif

//----------

#ifdef AX_FORMAT_LADSPA

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
// AX_ENTRYPOINT
// using main() here, to simplify example..

int main(void)
{

  //axFormatImpl<axDescriptor,axInstance,axPlatform,axInterface>* exe
  //  = new axFormatImpl<axDescriptor,axInstance,axPlatform,axInterface>();

  axFormatImpl<myDescriptor,myInstance,axPlatform,axInterface>*
    exe = new axFormatImpl<myDescriptor,myInstance,axPlatform,axInterface>();

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
