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

/**
 * \file axEffect.h
 * \brief effect class
*/

#ifndef axEffect_included
#define axEffect_included
//----------------------------------------------------------------------

#include "axPlugin.h"
#include "axEditor.h"
#include "axWidget.h"
#include "axVoice.h"

class axEffect : public axPlugin,
                 public axWidgetListener
{
  protected:
    axEditor* mEditor;

  public:

    axEffect(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        //setupEditor(AX_WIDTH,AX_HEIGHT);
        //setupEditor(mWidth,mHeight);
      }

    //----------

    //virtual ~axEffect()
    //  {
    //  }

    //--------------------------------------------------

    virtual void do_SetupEditor(axEditor* aEditor) {}
    virtual void do_IdleEditor(void) {}

    //--------------------------------------------------

    #define STR2(s) #s
    #define STR(s) STR2(s)
    #define WIN_NAME STR(AX_PLUGIN) "_window"

    virtual void* doCreateEditor(void)
      {
        axEditor* ED;
        ED = new axEditor(WIN_NAME,this,-1,axRect(0,0,mWidth,mHeight),AX_FLAGS);
        do_SetupEditor(ED);
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
        do_IdleEditor();
        if (mEditor) mEditor->redrawDirty();
      }

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
