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


//----------------------------------------------------------------------
//
// MAIN PAGE
//
//----------------------------------------------------------------------
/**  
  \image html logo.png

  \mainpage axonlib mainpage 

  <b>axonlib</b> is a standalone, multiplatform (win32/linux) library for
  vst plugins and applications, which focuses on the ease of
  use, while rataining 'low-level' control. it is realeased under the gnu lgpl
  license.
  <br>

  feature highlights:
  - gui widgets <br>
  - loading of external png images <br>
  - resizeable windows <br>
  - optimized math methods <br>
  - dsp classes <br>
  - lots of example plugins <br>
  <br>

  uses third party code by:
  - lode vandevenne <br>
  - jorgen aase <br>
  
  <b>pages:</b> <br>
  - \subpage installation
  - \subpage examples
  - \subpage compile
  - \subpage screenshots
  
*/
  
//----------------------------------------------------------------------
//
// INSTALL
//
//----------------------------------------------------------------------

/**
  \page installation installation
  <b>go to:</b>http://axonlib.googlecode.com <br>
   
  you get the latest revision from the svn reposotory or download the latest
  zip archive from the download section. <br>

  <b>get the vst sdk from:</b> <br>

  http://ygrabit.steinberg.de/~ygrabit/public_html/index.html <br>
  (the 2.4r1 link - it will actually forward to a registration page and you can get 2.4r2) <br>
  
  <br> please make sure you familiarize yourself with both axonlib and the vst sdk licenses. <br>

  <b>create the following folders:</b> <br>

  \code
  axonlib
  axonlib\axonlib <-- copy axonlib here (ax...bin...docs..etc)
  axonlib\vstsdk24 <-- copy vst sdk here (pluginterfaces...public.sdk...etc)
  \endcode

  <b>compiler requirements: </b>  
  
  <hr>
  
  <b>- linux:</b> <br>
  mingw32 - to cross-compile for win32 <br>
  g++ - to compile c++ code for linux <br>
  xorg-dev - required for gui-s. <br>
  <br>
  if you have ubuntu use: <br>
  \code
  sudo apt-get install mingw32
  sudo apt-get install g++
  sudo apt-get install xorg-dev
  \endcode
  
  <hr>
  
  <b>- win32:</b> <br>
  get mingw32 (the one included with dev-c++ will suffice) <br>
  set path to '[mingw32dir]/bin/g++.exe' <br>
  <br>
  dev-c++ - http://www.bloodshed.net/devcpp.html <br>
  mingw - http://www.mingw.org/ <br>
  
  <hr>
  
  <b>- cygwin:</b> <br> 
  -- to compile with cygwin for windows:: <br>
  download & install the standard mingw32, g++ packages <br>

  -- to compile with cygwin for linux:: <br>
  see bottom of 'axonlib/plugins/compile-cygwin' for instructions <br>
  
  you can get cygwin from: <br>
  http://www.cygwin.com/ <br>
  
  <br>
  
*/

//----------------------------------------------------------------------
//
// EXAMPLES
//
//----------------------------------------------------------------------

/**
  \page examples examples

  the absolutely simplest plugin you can make, a 'do nothing' plugin could look like this:

  \code
  #define AX_PLUGIN     myPlugin
  #define AX_NUMPROGS   1
  #define AX_NUMPARAMS  0
  #include "axPlugin.h"
  class myPlugin : public axPlugin
  {
    public:
      myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
        {
          describe("axPlugin_minimal","axAuthor","axProduct",0,0);
          setupAudio(0, 0);  // optional ?
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
      myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
        {
          describe("axPlugin_no_gui","axAuthor","axProduct",0,0);
          setupAudio(2, 2);
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

      myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
        {
          setupAudio(2, 2);
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
*/

//----------------------------------------------------------------------
//
// COMPILE
//
//----------------------------------------------------------------------

/**
  \page compile compilation

  there are ready to use compile scripts in the 'axonlib/plugins' folder.
  the compile scripts will move the resulted binaries to '../bin'.
  edit them to suit your needs.

  \note under win32 if you have a source file in a subfolder relative to 'compile.cmd'
  you should call the compile script like:
  \code
  cd mysubfolder
  ../compile.cmd myfx.cpp
  \endcode

  default compiler lines: <br>
  \code
  //win32::
  g++.exe -I../ax -I../../vstsdk24 -shared -mwindows -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //linux for linux::
  g++ -I../ax -I../../vstsdk24 -lX11 -lpthread -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //linux for win32::
  i586-mingw32msvc-g++ -I../ax -I../../vstsdk24 -mwindows -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //cygwin for win32::
  g++ -I../ax -I../../vstsdk24 -shared -mwindows -mno-cygwin -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //cygwin for linux::
  (see bottom of 'axonlib/plugins/compile-cygwin')
  \endcode

  (see 'man gcc' for more info) <br>
  
  suggested optimization flags:
  \code
  -O3 -Os -fdata-sections -ffunction-sections -funroll-loops
  \endcode

  for monitoring warnings use (note that the vst sdk 2.4 may pop a warning or two):
  \code
  -pedantic -fpermissive -W -Wall
  \endcode

  enabling sse optimized math. this will enable gas to optimize the generated asm code with sse and fpu instructions.
  \code
  -msse -mfpmath=sse,387
  \endcode
  <br>
  see this page for more information: <br>
  http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/i386-and-x86_002d64-Options.html
  <br><br>
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

*/

//----------------------------------------------------------------------
//
// SCREENSHOTS
//
//----------------------------------------------------------------------

/**
 \page screenshots screenshots
 
 <br>
 \image html logo.png <br>
 \image html logo.png <br>
 \image html logo.png <br> 
 */