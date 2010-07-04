#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#include "par/axParameter.h"
#include "par/axProgram.h"
#include "gui/axEditor.h"
#include "base/axInterface.h"

//----------------------------------------------------------------------

//// default names for (default) stereo in/out
//static char* g_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
//static char* g_stereo_outputs[] = { (char*)"out1",(char*)"out2" };

//----------------------------------------------------------------------

// not really used for exe's, but, perhaps later..
class axDescriptor
{
  private:
    axInterface* mInterface;
  public:
    axDescriptor(axInterface* aInterface) { mInterface = aInterface; }
    virtual ~axDescriptor() {}
    virtual axInterface* getInterface(void) { return mInterface; }
//    virtual char*         getName(void)             { return (char*)"plugin"; }
//    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
//    virtual char*         getProduct(void)          { return (char*)"unknown plugin"; }
//    virtual int           getVersion(void)          { return 0; }
//    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
//    virtual int           getNumInputs(void)        { return 2; }
//    virtual int           getNumOutputs(void)       { return 2; }
//    virtual int           getNumParams(void)        { return 0; }
//    virtual int           getNumProgs(void)         { return 0; }
//    virtual char*         getInputName(int aIndex)  { return g_stereo_inputs[aIndex]; }
//    virtual char*         getOutputName(int aIndex) { return g_stereo_outputs[aIndex]; }
//    virtual char*         getParamName(int aIndex)  { return (char*)"param"; }
};

//----------------------------------------------------------------------

class axInstance
{
  protected:
    axDescriptor* mDescriptor;
    axRect        mEditorRect;
  public:
    axInstance(axDescriptor* aDescriptor)
      {
        mDescriptor = aDescriptor;
        mEditorRect = axRect(0,0,256,256);
      }
    virtual ~axInstance() {}

    //----------

    virtual axDescriptor* getDescriptor(void) { return mDescriptor; }
    virtual axRect getEditorRect(void) { return mEditorRect; }

    //----------

    // exe
    virtual int main(int argc, char** argv)
      {
        //#ifndef AX_NOGUI
        //axInterface* interface = mDescriptor->getInterface();   trace("interface: " << interface);
        //Display* display = (Display*)interface->getHandle();    trace("display: " << display);
        //Window parent = XDefaultRootWindow(display);            trace("parent: " << parent);
        //axWindow* window = (axWindow*)doOpenEditor(interface,&parent);
        ////axWindow* win = (axWindow*)window;
        ////win->eventLoop();
        //#endif
        return 0;
      }

    //----------

    // vst
    virtual int   getCurrentProgram(void) { return 0; }
    virtual void  saveProgram(int aNum) {}

    virtual void* doOpenEditor(axInterface* aInterface, void* aParent) { return NULL; }
    virtual void  doCloseEditor(axInterface* aInterface, void* aParent) {}

};

//----------------------------------------------------------------------
//
// format implementation
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
        mPlatform   = new _P(this);
        mInterface  = new _In(mPlatform);
        mDescriptor = new _D(mInterface);
        mInstance   = new _I(mDescriptor);
        // audio?
      }
    virtual ~axFormatImpl()
      {
        delete mPlatform;
        delete mDescriptor;
        delete mInstance;
        delete mInterface;
      }
  //protected:
  public:
    virtual axPlatform*   getPlatform(void)   { return mPlatform; }
    virtual axDescriptor* getDescriptor(void) { return mDescriptor; }
    virtual axInstance*   getInstance(void)   { return mInstance; }
    virtual axInterface*  getInterface(void)  { return mInterface; }
    virtual char*         getName(void)       { return (char*)"exe"; }
};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

// calls instance.main

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
