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

#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------

//#define AX_WIDGET_NOUPDATELIST

#include "axFormat.h"
#include "core/axArray.h"
#include "gui/axWindow.h"
#include "skins/axSkinBasic.h"

//----------------------------------------------------------------------
//
// connection
//
//----------------------------------------------------------------------

// widget <-> parameter connection
struct wp_connection
{
  axWidget*     mWidget;
  axParameter*  mParameter;
  wp_connection(axWidget* aWidget, axParameter* aParameter)
    {
      mWidget=aWidget;
      mParameter=aParameter;
    }
};

typedef axArray<wp_connection> wp_connections;

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class axEditor : public axWindow
{
  protected:
    axFormat*       mFormat;
    wp_connections  mConnections;
    axSkinBasic*    mDefaultSkin;

  public:

    axEditor(axFormat* aFormat, axContext* aContext, axRect aRect, int aWinFlags)
    : axWindow(aContext,aRect,aWinFlags)
      {
        mFormat = aFormat;
        axCanvas* canvas = getCanvas();
        mDefaultSkin = new axSkinBasic(canvas);
        applySkin(mDefaultSkin);

      }

    //----------

    virtual ~axEditor()
      {
        delete mDefaultSkin;
      }

    //--------------------------------------------------

    inline axSkin* defaultSkin(void) { return mDefaultSkin; }

    //--------------------------------------------------

    void connect(axWidget* aWidget, axParameter* aParameter)
      {
        int conn = mConnections.size();
        aWidget->setConnection(conn);
        aWidget->setParameter(aParameter);
        aParameter->setConnection(conn);
        aWidget->setValue( aParameter->doGetValue() );
        mConnections.append( wp_connection(aWidget,aParameter) );   // ??? delete?
      }

    //void deleteConnections(void)
    //  {
    //  }

    //--------------------------------------------------

    //[internal]
    inline void internal_redraw(axWidget* aWidget)
    //inline void internal_redraw(axWidget* aWidget, bool fromgui/*=false*/)
      {
        //trace("internal_redraw");
        #ifdef AX_FORMAT_EXE
          redrawWidget(aWidget);
        #else
          #ifndef AX_WIDGET_NOUPDATELIST
            appendUpdate(aWidget);
          #else
            redrawWidget(aWidget);
          #endif
        #endif
      }

    //----------

    //virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
    //virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    //virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }

    //----------

    // called from axFormat.onChange(axParameter) if editor is open
    //
    // when the vast host automates a parameter, the parameter's onChange is
    // called (via axFormatVst.setParameter)..
    // this func is then called to redraw the widget.
    //
    // automation can theoretically come at any time, so we shouldn't just
    // blindly redraw the widget..
    // the internal_redraw function either sends an 'invalidate' notification
    // to the window/editor, or append the widget to an updatelist, and then this
    // will be redrawn in the next editor idle calls...

    virtual void paramChanged(axParameter* aParameter)
      {
        //trace("paramChanged");
        int conn = aParameter->getConnection();
        if (conn>=0)
        {
          axWidget* wdg = mConnections[conn].mWidget;
          float val = aParameter->doGetValue();
          wdg->setValue(val);
          internal_redraw(wdg); //redrawWidget(wdg);
        }
      }

    //----------

      // this one needs more work. specific hosts might need specific actions...
      // - axWindow.setSize()
      // - set size of rect that will be returned in effEditGetRect
      // - vst function sizeWindow()
      // various hosts react differently to these, and we need to do a host-check at startup,
      // and do more testing to see which host require what.

    void resizeWindow(int aWidth, int aHeight)
      {
        //trace("axEditor.resizeWindow: " << aWidth << "," << aHeight);
        mRect.w = aWidth;
        mRect.h = aHeight;
        // axWindow::setSize not needed for reaper, (effEditRect)
        // but if commented out, our window woon't be told to resize
        // same thing with energy xt2 (it supports sizeWindow
        // (only if host doesn't do any resizing?)
        // standalone needs it..
        // todo: #ifdef AX_FORMAT_...
        //#ifdef AX_FORMAT_EXE
        axWindow::setSize(aWidth,aHeight); // double resize!
        //#endif
        //#ifdef AX_FORMAT_VST
        //resizeBuffer(aWidth,aHeight);
        //doSetSize(aWidth,aHeight);
        //#endif
//        mFormat->notifyResizeEditor(aWidth,aHeight);
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        //trace("axEditor.doSetSize: " << aWidth << "," << aHeight);
        if (mFormat) mFormat->notifyResizeEditor(aWidth,aHeight);
        axWindow::doSetSize(aWidth,aHeight);
      }

    //--------------------------------------------------
    // on..
    //--------------------------------------------------

    //TODO: if tweaked from gui, redraw directly
    // else append to updatelist

    // this is called from a widget when its internal state has changed.
    // we notify the parameter, if there's any associated with the widget,
    // and redraw the widget

    virtual void onChange(axWidget* aWidget)
      {
        //trace("onChange wdg");
        int conn = aWidget->getConnection();
        if (conn>=0)
        {
          axParameter* par = mConnections[conn].mParameter;
          float val = aWidget->getValue();
          mFormat->notifyParamChanged(par);
          //par->doSetValue(val,false);
          par->doSetValue(val,true);
        }
        //internal_redraw(aWidget);
        // redraw directly (don't add to updatelist)
        // check possibility, feasability of drawing directly
        // via the editor's canvas, so we avoid the extra
        // 'invalidate' roundtrip to the os and back
        redrawWidget(aWidget);

      }

    //----------

    virtual void onRedraw(axWidget* aWidget)
      {
        //trace("onRedraw");
        internal_redraw(aWidget);
      }

    //----------

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
      {
        if (aMode<0)
        {
          axRect R = mFormat->getEditorRect();
          int w = R.w + aDeltaX; //mRect.w + aDeltaX;
          int h = R.h + aDeltaY; //mRect.h + aDeltaY;
          //axWidget::doSetSize(w,h);
          //resizeWindow( R.w + aDeltaX, R.h + aDeltaY );
          resizeWindow(w,h);
        }
        else axWindow::onSize(aWidget,aDeltaX,aDeltaY,aMode);
      }


};

//----------------------------------------------------------------------
#endif

