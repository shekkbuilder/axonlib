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
  \file axVoice.h
  \brief polyphonic voice manager
 */

#ifndef axSynth_included
#define axSynth_included
//----------------------------------------------------------------------

#include "axPlugin.h"
#include "axEditor.h"
#include "axWidget.h"
#include "axVoice.h"

class axSynth : public axPlugin,
                public axWidgetListener
{
  protected:
    axVoiceManager* VM;
    axEditor*       mEditor;

  public:

    axSynth(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        VM = new axVoiceManager();
        setupEditor(AX_WIDTH,AX_HEIGHT);
      }

    //----------

    virtual ~axSynth()
      {
        delete VM;
      }

    //--------------------------------------------------

    virtual void doSetupEditor(axEditor* aEditor) {}
    //virtual void do_IdleEditor(void) {}

    //--------------------------------------------------
    // signals
    //--------------------------------------------------

    virtual void doProcessState(int aState)
      {
        switch(aState)
        {
          case pst_Resume:
            VM->postProcess(); // to be sure there's no unprocessed events in the buffers
            break;
        }
      }

    //virtual void doProcessTransport(int aState) {}

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        VM->midi(ofs,msg1,msg2,msg3);
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    {
      //updateTimeInfo();
      #ifndef AX_AUTOSYNC
        (void)getSampleRate();
      #endif
      VM->setSampleRate( mSampleRate );
      VM->preProcess();
      return false;
    }

    //----------

    //TODO: stereo
    virtual void doProcessSample(float** ins, float** outs)
      {
        //float out = VM->process();
        //float out = VM->process();
        float _outs[2];
        VM->process(&_outs[0]);
        *outs[0] = _outs[0];
        *outs[1] = _outs[1];
      }

    //----------

    virtual void doPostProcess(float** inputs, float** outputs, long sampleFrames)
      {
        VM->postProcess();
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    //#define STRINGIZE2(s) #s
    //#define STRINGIZE(s) STRINGIZE2(s)
    //#define WINCLASS STRINGIZE(AX_PLUGIN) "_window"
    #define STR2(s) #s
    #define STR(s) STR2(s)
    #define WIN_NAME STR(AX_PLUGIN) "_window"

    virtual void* doCreateEditor(void)
      {
        //trace(WINCLASS);
        //trace( "strlen " << strlen(WINCLASS) );
        //trace( (int)WINCLASS[14] );
        axEditor* ED = new axEditor(WIN_NAME,this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        doSetupEditor(ED);
        //ED->doRealign();
        mEditor = ED;
        return mEditor;
      }

    #undef STR
    #undef STR2
    #undef WIN_NAME

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* ED = mEditor;
        mEditor = NULL;
        delete ED;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        //do_IdleEditor();
        if (mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------
    // listeners
    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);

      }

};


//----------------------------------------------------------------------
#endif
