#ifndef axDescriptor_included
#define axDescriptor_included
//----------------------------------------------------------------------

// default names for (default) stereo in/out
static char* g_default_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
static char* g_default_stereo_outputs[] = { (char*)"out1",(char*)"out2" };

//----------------------------------------------------------------------

class axDescriptor
{
  public:
    //axDescriptor(axBase* aBase)  { /*trace("axDescriptor.constructor");*/ }
    //virtual ~axDescriptor()      { /*trace("axDescriptor.destructor");*/ }
    //
    virtual char*         getName(void)             { return (char*)"plugin"; }
    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
    virtual char*         getProduct(void)          { return (char*)"unknown plugin"; }
    virtual int           getVersion(void)          { return 0; }
    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
    virtual int           getNumInputs(void)        { return 2; }
    virtual int           getNumOutputs(void)       { return 2; }
    virtual int           getNumParams(void)        { return 0; }
    virtual int           getNumProgs(void)         { return 0; }
    virtual char*         getInputName(int aIndex)  { return g_default_stereo_inputs[aIndex]; }
    virtual char*         getOutputName(int aIndex) { return g_default_stereo_outputs[aIndex]; }
    virtual char*         getParamName(int aIndex)  { return (char*)"param"; }
    //
    virtual bool          isSynth(void)             { return false; }
    virtual bool          hasEditor(void)           { return false; }
    virtual axRect        getEditorRect(void)       { return axRect(0,0,100,100); }
};

//----------------------------------------------------------------------
#endif
