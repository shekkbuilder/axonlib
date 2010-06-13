/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

/*
NOTES:
- if you override doIdleEditor(), you have to call axFormat::doIdleEditor
  to have the widgets repainted (unless you have AX_WIDGET_NOUPDATELIST defined),
  or do something like this yoursef in your idle function:
    if (mEditorWindow) mEditorWindow->redrawUpdates();
*/

//TODO: updateList (dirtyParameters)

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axAssert.h"
#include "core/axUtils.h"
#include "platform/axContext.h"
//#include "base/axFormatBase.h"
#include "format/axFormatBase.h"

#include "par/axParameter.h"
#include "gui/axWindow.h"
//#include "gui/axWidget.h"

//----------

// #define AX_FORMAT_EXE or #define AX_FORMAT_VST, etc..
//  either on the compiler command line (or compiler scripts),
// or in your ide's build target setup

//----------

#ifdef AX_FORMAT_VST
  #include "format/axFormatVst.h"
  //#define AX_WIN_DEFAULT AX_WIN_PLUGDEFAULT
#endif

#ifdef AX_FORMAT_LADSPA
  #include "format/axFormatLadspa.h"
  //#define AX_WIN_DEFAULT AX_WIN_PLUGDEFAULT
#endif

#ifdef AX_FORMAT_EXE
  #include "format/axFormatExe.h"
  //#define AX_WIN_DEFAULT AX_WIN_APPDEFAULT
#endif

//----------------------------------------------------------------------

class axFormat : public axFormatImpl, public axParameterListener
{
  protected:
    axSystemInfo mSystemInfo;
  public:
    axFormat(axContext* aContext,int aFormatFlags=ff_None)
    //: axFormatBase(aContext,aFormatFlags)
    : axFormatImpl(aContext,aFormatFlags)
      {
        // auto open debug (win32)
        #if defined AX_DEBUG && defined AX_WIN32
          #ifdef AX_DEBUG_AUTO_STD
            axDstdCreate();
          #endif
          #ifdef AX_DEBUG_AUTO_WIN
            axDwinCreate();
          #endif
        #endif

        // open fstream (debug)
        #if defined AX_DEBUG_LOG && defined AX_DEBUG
          char filepath[AX_MAX_PATH] = "";
          axGetBasePath(filepath);
          axStrcat(filepath, AX_DEBUG_LOG);
          trace("axDlog, logfile: " << filepath);
          axDlog.open(filepath, ios::out);
          axAssert(axDlog.is_open());
        #endif

      }

    virtual ~axFormat()
      {
        #ifndef AX_NOAUTODELETE
          deleteParameters();
          deletePrograms();
        #endif

        // auto close debug (win32)
        #if defined AX_DEBUG && defined AX_WIN32
          #ifdef AX_DEBUG_AUTO_WIN
            axDwinDestroy();
          #endif
          #ifdef AX_DEBUG_AUTO_STD
            axDstdDestroy();
          #endif
        #endif

      }

    //--------------------------------------------------

    virtual axSystemInfo* getSystemInfo(void)
      {
        #ifdef AX_LINUX
          mSystemInfo.os = "linux";
        #endif
        #ifdef AX_WIN32
          mSystemInfo.os = "win32";
        #endif
        mSystemInfo.cpuid    = axCpuCaps();
        mSystemInfo.cpuidstr = axCpuCapsString();
        mSystemInfo.bigend   = axBigEndian();

        //TODO: screen width/height/depth
        //      especially needed on linux, since blitting (copyarea) crashes
        //      with differing bit-depths (sometimes)

        return &mSystemInfo;
      }

    //--------------------------------------------------

    virtual void appendParameter(axParameter* aParameter)
      {
        int index = mParameters.size();
        aParameter->setIndex(index);
        mParameters.append(aParameter);
      }

    //----------

    virtual void deleteParameters(void)
      {
        for (int i=0; i<mParameters.size(); i++) delete mParameters[i];
      }

    //--------------------------------------------------

    virtual void appendProgram(axProgram* aProgram)
      {
        //int index = mPrograms.size();
        //aProgram->setIndex(index);
        mPrograms.append(aProgram);

//          trace("appendProgram: " <<
//            aProgram->getValue(0) << "," <<
//            aProgram->getValue(1) << "," <<
//            aProgram->getValue(2) << "," <<
//            aProgram->getValue(3) << "," <<
//            aProgram->getValue(4) << "," <<
//            aProgram->getValue(5) << "," <<
//            aProgram->getValue(6) << "," <<
//            aProgram->getValue(7) << "," <<
//            aProgram->getValue(8) );

      }

    //----------

    virtual void deletePrograms(void)
      {
        for (int i=0; i<mPrograms.size(); i++) delete mPrograms[i];
      }

    //--------------------------------------------------

    virtual axProgram* createDefaultProgram(void)
      {
        int num = mParameters.size();
        axProgram* prog = new axProgram("default",num);
        for (int i=0; i<num; i++)
        {
          float val = mParameters[i]->doGetValue();
          prog->setValue(i,val);
        }
        return prog;
      }

    //----------

    virtual void saveProgram(int aIndex)
      {
        //trace("saveProgram");
        if (mPrograms.size() > 0)
        {
          int num = mParameters.size();
          axProgram* prog = mPrograms[aIndex];
          for (int i=0; i<num; i++)
          {
            float val = mParameters[i]->doGetValue();
            prog->setValue(i,val);
          }
        }
      }

    virtual void loadProgram(int aIndex)
      {
        //trace("loadProgram");
        if (mPrograms.size() > 0)
        {
          int num = mParameters.size();
          axProgram* prog = mPrograms[aIndex];
          for (int i=0; i<num; i++)
          {
            //float val = mParameters[i]->doGetValue();
            //prog->setValue(i,val);
            float val = prog->getValue(i);
            //trace("i: "<<i<<" val: "<<val);
            mParameters[i]->doSetValue(val,true);
          }
        }
      }

    //--------------------------------------------------

    virtual void doIdleEditor(void)
      {
        #ifndef AX_WIDGET_NOUPDATELIST
        //trace("axFormat.doIdleEditor");
        if (mEditorWindow) mEditorWindow->redrawUpdates();
        #endif
      }

    // from plugin

    virtual void onChange(axParameter* aParameter)
      {
        //trace("onChange");
        doSetParameter(aParameter);
        if (mEditorOpen && mEditorWindow) mEditorWindow->paramChanged(aParameter);
      }

};

//----------------------------------------------------------------------
#endif
