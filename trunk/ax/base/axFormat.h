#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

#include "base/axBase.h"

//----------------------------------------------------------------------

#ifdef AX_FORMAT_EXE
#include "base/axFormatExe.h"
#endif

#ifdef AX_FORMAT_VST
#include "base/axFormatVst.h"
#endif

#ifdef AX_FORMAT_LADSPA
#include "base/axFormatLadspa.h"
#endif

//----------------------------------------------------------------------
#endif


























//----------------------------------------------------------------------


//#ifndef axFormat_included
//#define axFormat_included
////----------------------------------------------------------------------
//
//#include "axConfig.h"
//#include "core/axDefines.h"
//#include "core/axMalloc.h"
//#include "core/axDebug.h"
//#include "core/axAssert.h"
//#include "core/axRect.h"
//#include "par/axParameter.h"
//
////----------------------------------------------------------------------
//
//// format types
//#define ft_Unknown  0
//#define ft_Exe      1
//#define ft_Vst      2
//#define ft_Ladspa   3
////#define ft_Dssi     4
////#define ft_Lv2      5
////#define ft_Au       6
//
//// instance states
//#define is_Unknown  0
//#define is_Open     1
//#define is_Close    2
//#define is_Suspend  3
//#define is_Resume   4
//#define is_Rate     5
//#define is_Block    6
//
//// instance flags
//#define if_None       0
//#define if_HasEditor  1
//
////----------------------------------------------------------------------
//
//class axFormatBase
//{
//  public:
//};
//
//// axPlatformBase
//// axInterfaceBase
//
////----------
//
//class axDescriptorBase
//{
//  public:
//    virtual char*         getName(void)             { return (char*)"plugin"; }
//    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
//    virtual char*         getProduct(void)          { return (char*)"unknown plugin"; }
//    virtual int           getVersion(void)          { return 0; }
//    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
//    virtual int           getNumInputs(void)        { return 2; }
//    virtual int           getNumOutputs(void)       { return 2; }
//    virtual int           getNumParams(void)        { return 0; }
//    virtual int           getNumProgs(void)         { return 0; }
//    virtual char*         getInputName(int aIndex)  { return NULL; }
//    virtual char*         getOutputName(int aIndex) { return NULL; }
//    virtual char*         getParamName(int aIndex)  { return NULL; }
//  //virtual char*         getProgName(int aIndex)   { return NULL; }
//};
//
////----------
//
//class axInstanceBase : public axParameterListener
//{
//  public:
//    virtual void    doStateChange(int aState) {}
//    virtual void    doProcessTransport(int aState) {}
//    virtual void    doProcessMidi(int aOffset, unsigned char aMsg1, unsigned char aMsg2, unsigned char aMsg3) {}
//    virtual void    doSetParameter(axParameter* aParameter) {}
//    virtual void    doPreProgram(int aProgram) {}
//    virtual void    doSetProgram(int aProgram) {}
//    virtual bool    doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize) { return false; }
//    virtual void    doProcessSample(SPL** aInputs, SPL** aOutputs) {}
//    virtual void    doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize) {}
//    virtual void*   doOpenEditor(void* aParent) { return NULL; }
//    virtual void    doCloseEditor(void) {}
//    virtual void    doIdleEditor(void) {}
//
//};
//
////----------------------------------------------------------------------
//
//#ifdef AX_FORMAT_EXE
//#include "base/axFormatExe.h"
//#endif
//
//#ifdef AX_FORMAT_VST
//#include "base/axFormatVst.h"
//#endif
//
//#ifdef AX_FORMAT_LADSPA
//#include "base/axFormatLadspa.h"
//#endif
//
////----------------------------------------------------------------------
//
//#define DEFAULT_DESCRIPTOR axDescriptor
//#define DEFAULT_INSTANCE   axInstance
//
////----------------------------------------------------------------------
//#endif

