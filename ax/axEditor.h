#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------

#define AX_WIDGET_NOUPDATELIST

//TODO: updateList (dirtyWidgets)

#include "axPlugin.h"
#include "core/axArray.h"
#include "gui/axWindow.h"
//#include "gui/axSkinDefault.h"

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
    axPlugin*       mPlugin;
    wp_connections  mConnections;
    axSkinDefault*  mDefaultSkin;
    #ifndef AX_WIDGET_NOUPDATELIST
    axWidgets       mUpdateList;
    #endif
    //axWidget*       mModalWidget;
    //int             mModalIndex;

  public:

    axEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axWindow(aContext,aRect,aWinFlags)
      {
        mPlugin = aPlugin;
        axCanvas* can = getCanvas();
        mDefaultSkin = new axSkinDefault(can);
        applySkin(mDefaultSkin);
        //mModalWidget = NULL;
        //mModalIndex  = -1;

      }

    virtual ~axEditor()
      {
        delete mDefaultSkin;
      }

    //----------

    void connect(axWidget* aWidget, axParameter* aParameter)
      {
        int conn = mConnections.size();
        aWidget->setConnection(conn);
        aWidget->setParameter(aParameter);
        aParameter->setConnection(conn);
        aWidget->setValue( aParameter->doGetValue() );
        mConnections.append( wp_connection(aWidget,aParameter) );
      }

    //void deleteConnections(void)
    //  {
    //  }

    //----------

    // only used internally (window/editor?)

    //virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
    //virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    //virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }

    // called from axEditor.onChange(axParameter) if editor is open
    virtual void paramChanged(axParameter* aParameter)
      {
        int conn = aParameter->getConnection();
        if (conn>=0)
        {
          axWidget* wdg = mConnections[conn].mWidget;
          float val = aParameter->doGetValue();
          wdg->setValue(val);
          redrawWidget(wdg);
        }
      }

    //----------------------------------------

      // this one needs more work. specific hosts might need specific actions...
      // - axWindow.setSize()
      // - set size of rect that will be returned in effEditGetRect
      // - vst function sizeWindow()
      // various hosts rfeact differently to these, and we need to do a host-check at startup,
      // and do more testing to see which host require what.

    void resizeWindow(int aWidth, int aHeight)
      {
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
        mPlugin->notifyResizeEditor(aWidth,aHeight);
      }

    //----------------------------------------

    /*
      axArray.append looks like can be dangerous,
      because it increases the size before the item itself is added to the array..
      if a thread is reading the array size just after it has been increased,
      but before the item has been properly set up and written to the array buffer,
      things could go wrong...
    */

    #ifndef AX_WIDGET_NOUPDATELIST

    void clearUpdates(void)
      {
        //mutex_dirty.lock();
        mUpdateList.clear(false);
        //mutex_dirty.unlock();
      }

    //----------

    void appendUpdate(axWidget* aWidget)
      {
        for( int i=0; i<mUpdateList.size(); i++ ) if( mUpdateList[i]==aWidget ) return;
        //mutex_dirty.lock();
        mUpdateList.append(aWidget);
        //mutex_dirty.unlock();
      }

    //----------

    // if we're inside this redrawDirty (because of idleEditor),
    // we can't append new widgets to it!!
    // dangerous if we don't manage the redrawDirty ourselves...
    // we might need a redrawLock here
    // or we need to be very certain about which thread is adding
    // widgets to the array, and which is reading from the list...
    // appendUpdate vs redrawUpdates

    void redrawUpdates(void)
      {
        //mutex_dirty.lock();
        int num = mUpdateList.size();
        for( int i=0; i<num; i++ )
        {
          axWidget* wdg = mUpdateList[i];
          redrawWidget(wdg);
        }
        clearUpdates();
        //mutex_dirty.unlock();
        //flush();

      }

    #endif // AX_WIDGET_NOUPDATELIST

    //----------------------------------------

    //[internal]
    inline void internal_redraw(axWidget* aWidget)
      {
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

    //--------------------------------------------------
    // do
    //--------------------------------------------------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        if (mPlugin) mPlugin->notifyResizeEditor(aWidth,aHeight);
        axWindow::doSetSize(aWidth,aHeight);
      }

    //----------

//    virtual void doIdleEditor()
//      {
//        #ifndef AX_WIDGET_NOUPDATELIST
//        if (mEditor) mEditor->redrawUpdates();
//        #endif
//        //trace("doIdleEditor");
//      }

    //--------------------------------------------------
    // on
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        int conn = aWidget->getConnection();
        if (conn>=0)
        {
          axParameter* par = mConnections[conn].mParameter;
          float val = aWidget->getValue();
          mPlugin->notifyParamChanged(par);
          par->doSetValue(val);
        }
        internal_redraw(aWidget);
      }

    //----------

    virtual void onRedraw(axWidget* aWidget)
      {
        internal_redraw(aWidget);
      }

    //----------

    //virtual void onCursor(int aCursor) { setCursor(aCursor); }
    //virtual void onHint(axString aHint) {}
    //virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY)

};

//----------------------------------------------------------------------
#endif

