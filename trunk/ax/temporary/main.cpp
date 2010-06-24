#define AX_LINUX
//#define AX_FORMAT_VST
//#define AX_NOGUI

//#include "core/axDefines.h"
//#include "core/axDebug.h"

#include "axFormat.h"

//----------------------------------------------------------------------

/*

  global/static names for parameters, inputs & outputs

*/

static char* g_inputs[]  = { (char*)"in1", (char*)"in2" };
static char* g_outputs[] = { (char*)"out1",(char*)"out2" };
static char* g_params[]  = { (char*)"gain" };

//----------------------------------------------------------------------

/*

  this class is created once, when the main entrypoint function is called,
  after the host loads the ddll/so.
  handles things that the host want to know about the plugin before any
  instances is created. the plugin. name, number of parameters, etc..

*/

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axPlatform* aPlatform) : axDescriptor(aPlatform) {}
    virtual char* getName(void)             { return (char*)"plugin"; }
    virtual char* getAuthor(void)           { return (char*)"author"; }
    virtual char* getProduct(void)          { return (char*)"product string"; }
    virtual int   getVersion(void)          { return 1; }
    virtual int   getUniqueId(void)         { return 0x11223344; }
    virtual int   getNumInputs(void)        { return 2; }
    virtual int   getNumOutputs(void)       { return 2; }
    virtual int   getNumParams(void)        { return 1; }
    virtual int   getNumProgs(void)         { return 0; }
    virtual char* getInputName(int aIndex)  { return g_inputs[aIndex]; }
    virtual char* getOutputName(int aIndex) { return g_outputs[aIndex]; }
    virtual char* getParamName(int aIndex)  { return g_params[aIndex]; }
};

//----------------------------------------------------------------------

/*

  this is the actual plugin...
  quite similar to what was our main plugin class, except that the plugin
  info/description (enumeration info) has moved to axDescriptor.

*/

class myInstance : public axInstance
{
  public:
    //myInstance() : axInstance() {}
    myInstance(axDescriptor* aDescriptor) : axInstance(aDescriptor) {}
    virtual int main(int argc, char** argv) { return 0; } // only called for standalone EXEs
    // virtual void doProcessSample(SPL** aInputs, SPL** aOutputs) {}
    // virtual void doSetParameter(axParameter* aParameter) {}
    // ...
};

//----------------------------------------------------------------------

/*

  editor

*/

//class myEditor : public axEditor
//{
//  public:
//    myEditor(axInstance* aInstance) : axEditor(aInstance) {}
//};

//----------------------------------------------------------------------

/*

  main/editor

*/

// AX_NOGUI
// AX_MAIN(myDescriptor,myInstance)

AX_MAIN(myDescriptor,myInstance,axInterface)

//AX_ENTRYPOINT(axDescriptor,axInstance,axPlatform)
//AX_ENTRYPOINT(myDescriptor,myInstance,myPlatform)


