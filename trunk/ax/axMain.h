/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
  \file axMain.h
  \brief main application/plugin entry point.

  tries to take care of how he different platforms work with loadable libraries (dll/so),
  and hide the differences between a plugin and a standalone binary
*/

//----------------------------------------------------------------------
// main help page doxs
//----------------------------------------------------------------------

/**
  \mainpage axonlib mainpage
  \section introduction_section # introduction
  introduction.
  
  \section install_section # installation   
  get the latest revisions from the svn reposotory at: <br>    
  http://axonlib.googlecode.com <br>
  
  <b>get the vst sdk from:</b> <br>
  
  http://ygrabit.steinberg.de/~ygrabit/public_html/index.html <br> 
  (the 2.4r1 link) <br>
  
  create the following folders: <br>
  
  \code
  axonlib 
  axonlib\axonlib <-- copy axonlib here (ax...bin...docs..etc) 
  axonlib\vstsdk24 <-- copy vst sdk here (pluginterfaces...public.sdk...etc) 
  \endcode
  
  <b>compilers and requirments: </b> <br>
  
  -- for linux download & install:: <br>  
  mingw32 - to cross-compile for win32 <br>
  g++ - to compile c++ code for linux <br>
  xorg-dev - required for gui-s. <br>
  
  -- win32:: <br>
  get mingw32 (the one with <br>
  set path to '[mingw32dir]\bin\g++.exe' <br>

  -- to compile with cygwin for windows:: <br>
  download & install the standard mingw32, g++ packages <br>
  
  -- to compile with cygwin for linux:: <br>
  see bottom of 'axonlib\plugins\compile-cygwin' for instructions <br>
  
  \section usage_section # usage

  the absolutely simplest plugin you can make, a 'do nothing' plugin could look like this:

  \code
  #define AX_PLUGIN     myPlugin
  #define AX_NUMPROGS   1
  #define AX_NUMPARAMS  0
  #include "axPlugin.h"
  class myPlugin : public axPlugin
  {
    public:
      myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
        {
          describe("axPlugin_minimal","axAuthor","axProduct",0,0);
        }
  };
  #include "axMain.h"
  \endcode

  template for non-gui plugin:

  \code
  #define AX_PLUGIN     myPlugin
  #define AX_NUMPROGS   1
  #define AX_NUMPARAMS  1
  //#define AX_AUTOSYNC
  #define AX_DEBUG
  #include "axDebug.h"
  #include "axPlugin.h"
  #include "parFloat.h"

  class myPlugin : public axPlugin
  {
    public:
      float mValue;
    public:
      myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
        {
          describe("axPlugin_no_gui","axAuthor","axProduct",0,0);
          //isSynth();
          appendParameter( new parFloat(this,0,"value","",0.5) );
          processParameters();
        }

      virtual ~myPlugin()
        {
        }

    // 1=open, 2=close, 3=suspend, 4=resume
    virtual void doProcessState(int aState)
      {
      }

    // (if AX_AUTOSYNC). 1=changed, 2=playing, 4=cycle, 8=rec, 64=write, 128=read
    virtual void doProcessTransport(int aState)
      {
      }

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
      }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mValue = f; break;
        }
      }

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //updateTimeInfo(); // (if !AX_AUTOSYNC)
        return false;
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0 * mValue;
        *outs[1] = spl1 * mValue;
      }

  };
  #include "axMain.h"
  \endcode

  template for gui plugin:

  \code
  #define AX_PLUGIN     myPlugin
  #define AX_NUMPROGS   1
  #define AX_NUMPARAMS  1
  #define AX_WIDTH      320
  #define AX_HEIGHT     240
  #define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
  //#define AX_AUTOSYNC
  //#define AX_NODIRTYWIDGETS
  //#define AX_PAINTERS
  #define AX_DEBUG
  #include "axDebug.h"
  #include "axPlugin.h"
  #include "parFloat.h"
  #include "axEditor.h"
  #include "wdgKnob.h"

  class myPlugin : public axPlugin,
                   public axWidgetListener
  {
    private:
      float     mValue;
      parFloat* pValue;
      axEditor* mEditor;
      bool      mGuiInitialized;
      wdgKnob*  wValue;

    public:

      myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
        {
          mEditor = NULL;
          mGuiInitialized = false;
          describe("axPlugin_with_gui","axAuthor","axProduct",0,0);
          hasEditor(AX_WIDTH,AX_HEIGHT);
          //isSynth();
          appendParameter(pValue=new parFloat(this,0,"value","",0.5));
          processParameters();
        }

      virtual ~myPlugin()
        {
            if (mGuiInitialized) {}
        }

      virtual void doProcessState(int aState)
        {
        }

      virtual void doProcessTransport(int aState)
        {
        }

      virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
        {
        }

      virtual void doProcessParameter(axParameter* aParameter)
        {
          int  id = aParameter->mID;
          float f = aParameter->getValue();
          switch(id)
          {
            case 0: mValue = f;  break;
          }
        }

      virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
        {
          return false;
        }

      virtual void doProcessSample(float** ins, float** outs)
        {
          float spl0 = *ins[0];
          float spl1 = *ins[1];
          *outs[0] = spl0;
          *outs[1] = spl1;
        }

      virtual void* doCreateEditor(void)
        {
          axEditor* EDIT = new axEditor("with_gui_win",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
          if (!mGuiInitialized)
          {
            // do one-time things here (png-decoding, ..)
            mGuiInitialized = true;
          }
          EDIT->appendWidget( wValue = new wdgKnob(this,0,axRect(100,100,128,32),wal_None) );
          EDIT->connect(wValue,pValue);
          EDIT->doRealign();
          mEditor = EDIT;
          return (void*)mEditor;
        }

      virtual void doDestroyEditor(void)
        {
          axEditor* EDIT = mEditor;
          mEditor = NULL;
          delete EDIT;
        }

      virtual void doIdleEditor(void)
        {
          if (mEditor) mEditor->redrawDirty();
        }

      virtual void onChange(axParameter* aParameter)
        {
          if (mEditor) mEditor->onChange(aParameter);
          doProcessParameter(aParameter);
        }

      virtual void onChange(axWidget* aWidget)
        {
          if (mEditor) mEditor->onChange(aWidget);
        }

  };

  #include "axMain.h"
  \endcode

  \section compile_section # compilation
  
  there ready to use compile scripts in the 'axonlib/plugins' folder.
  the compile scripts will move the resulted binaries to '../bin'
  edit them suit your needs.
   
  \note under win32 if you have a source file in a subfolder relative to 'compile.cmd'
  you should call the compile script like:
  \code
  cd mysubfolder
  ..\compile.cmd myfx.cpp
  \endcode  
  
  (see 'man gcc' for more info) <br>
  
  default compiler lines: <br>
  \code
  cygwin for win32::
  g++ -I../ax -I../../vstsdk24 -shared -mwindows -mno-cygwin -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  win32::
  g++.exe -I../ax -I../../vstsdk24 -shared -mwindows -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  linux for linux::
  g++ -I../ax -I../../vstsdk24 -lX11 -lpthread -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  linux for win32::  
  i586-mingw32msvc-g++ -I../ax -I../../vstsdk24 -mwindows -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  \endcode
      
  suggested optimization flags:
  \code
  -O3 -Os -fdata-sections -ffunction-sections -funroll-loops
  \endcode
  
  for monitoring warnings use (note that the vst sdk 2.4 may pop a warning or two):
  \code
  -pedantic -fpermissive -W -Wall
  \endcode
  
  enabling sse optimized math. this will enable gas to optimize the generated asm code with sse instructions. 
  \code 
  -msse -mfpmath=sse
  \endcode
  
  enabling 'fast-math' will allow the compiler to cut some corner when the default c math functions are used (sinf, tanf etc).
  this will make such as fast as the included methods in axMath.h but with reduced accuracy.
  \code
   -ffast-math
  \endcode
  
  stripping the generated binary: <br>
  (see 'man strip' for more info)
  \code
  strip --strip-all [binaryfile]
  //or (i586-mingw32msvc-strip)
  \endcode
  
  \subsection subsection1 a subsection

*/

//----------------------------------------------------------------------

#ifndef axMain_included
#define axMain_included
//----------------------------------------------------------------------

// TODO:
// - win32: window class?

#ifndef AX_NUMPROGS
  #define AX_NUMPROGS 1
#endif

#ifndef AX_NUMPARAMS
  #define AX_NUMPARAMS 0
#endif

#ifndef AX_FLAGS
  #define AX_FLAGS 0
#endif

//----------------------------------------------------------------------
//
// application
//
//----------------------------------------------------------------------

#ifdef AX_APPLICATION

  int main(void)
  {
    AX_APPLICATION* app = new AX_APPLICATION();
    //TODO: command line arguments
    app->main();
    return 0;
  }

#endif // AX_APPLICATION

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

#ifdef AX_PLUGIN

//#include "axHostVst.h"

  #ifdef WIN32

    /// DllMain
    /**
      called when windows load the dll
      saves the HINSTANCE variable, so we have it accessible
    */

    BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved)
    {
      gInstance = hModule;
      return TRUE;
    }

    /// main entry point
    /**
      called by the host when the dll/so is initially loaded.
      note that the dll/so will only be loaded once, and all instances of your plugin will share access to any global data
    */

    int main(audioMasterCallback audioMaster)
    {
      AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      //axHostVst* host = new axHostVst(audioMaster);
      //AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return (int)plugin->getAeffect();
    }

  #endif //Win32

  //----------

  #ifdef linux

    AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
    #define main main_plugin

    /// main entry point
    /**
      called by the host when the dll/so is initially loaded.
      note that the dll/so will only be loaded once, and all instances of your plugin will share access to any global data.
      (a little #define trickery is going on in the linux version, because of the calling convention used in the vst sdk)
    */

    AEffect* main(audioMasterCallback audioMaster)
    {
      AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      //axHostVst* host = new axHostVst(audioMaster);
      //AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return plugin->getAeffect();
    }

  #endif //linux

#endif //AX_PLUGIN

//----------------------------------------------------------------------
#endif
