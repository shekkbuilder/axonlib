#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------
// mainly does two things:
// - onChange(wdg/par) transfer mValue, widget <-> parameter (if mID>0)
// - keep track of 'dirty widgets'

#include "axParameter.h"
#include "axPlugin.h"
#include "axWindow.h"

#include "axMutex.h"

//----------------------------------------------------------------------

class axWPConnection
{
  public:
    axWidget    *mWidget;
    axParameter *mParameter;
  public:
    axWPConnection(axWidget *aWidget, axParameter *aParameter)
      {
        mWidget = aWidget;
        mParameter = aParameter;
        //TODO: transfer mValue
      }
};

//----------

typedef axArray<axWPConnection*> axWPConnections;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axEditor : public axWindow,
                 public axParameterListener
{
  private:
    axMutex   mutex_dirty;
  public:
    axPlugin        *mPlugin;
    axWidgets       mDirtyList;
    axWPConnections mConnections;

  public:

    axEditor(axString aWinName, axPlugin* aPlugin, int aID, axRect aRect, int aWinFlags=0)
    : axWindow(aWinName,this,aID,aRect,aWinFlags)
      {
        mPlugin = aPlugin;
      }

    //----------

    virtual ~axEditor()
      {
        //try to make sure there's no redraw messages pending
        //clearDirty();
        //flush();
        deleteConnections();
      }

    //----------------------------------------
    // dirty
    //----------------------------------------

    void clearDirty(void)
      {
        //mutex_dirty.lock();
        mDirtyList.clear(false);
        //mutex_dirty.unlock();
      }

    //----------

    void appendDirty(axWidget* aWidget)
      {
        for( int i=0; i<mDirtyList.size(); i++ ) if( mDirtyList[i]==aWidget ) return;
        //mutex_dirty.lock();
        mDirtyList.append(aWidget);
        //mutex_dirty.unlock();
      }

    //----------

    // if we're inside this redrawDirty (because of idleEditor),
    // we can't append new widgets to it!!
    // dangerous if we don't manage the redrawDirty ourselves...
    //
    // we might need a redrawLock here

    void redrawDirty(void)
      {
        //mutex_dirty.lock();
        //TRACE("redrawDirty\n");
        int num = mDirtyList.size();
        for( int i=0; i<num; i++ )
        {
          axWidget* wdg = mDirtyList[i];
          redrawWidget(wdg);
        }
        clearDirty();
        //TRACE("...redrawDirty ok\n");
        //mutex_dirty.unlock();
        //flush();

      }

    //----------------------------------------
    // connections
    //----------------------------------------

    void connect(axWidget* aWidget, axParameter* aParameter)
      {
        aWidget->mParameter = aParameter;
        int num = mConnections.size();
        aWidget->mCNum = num;
        aParameter->mCNum = num;
        aWidget->doSetValue( aParameter->doGetValue() );
        //lock
        mConnections.append( new axWPConnection(aWidget,aParameter) );
        //unlock
      }

    //----------

    void connectAll(void)
      {
      }

    //----------

    void deleteConnections(void)
      {
        //TRACE("- axEditor: deleteConnections()\n");
        //lock
        for(int i=0; i<mConnections.size(); i++) delete mConnections[i];
        mConnections.clear();
        //unlock
        //TRACE("- ..ok\n");
      }

    //----------------------------------------
    // widget handler
    //----------------------------------------

//    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
//      {
//        if (mPlugin->mEditorIsOpen) axWindow::doPaint(aCanvas,aRect);
//      }

    //----------------------------------------
    // widget listener
    //----------------------------------------

    // called from gui when tweaking widget
    virtual void onChange(axWidget* aWidget)
      {
        //TRACE("- axEditor: onChange(wdg)... id=%i\n",aWidget->mID);
        int con = aWidget->mCNum;
        //TRACE("wdg.con: %i\n",con);
        if( con>=0 )
        {
          axParameter* par = mConnections[con]->mParameter;
          float val = aWidget->doGetValue();
          //TRACE("  plugin=%x\n",(int)mPlugin);
          //mPlugin->setParameterAutomated(con,val); // will call back to onChange below (to be redrawn)
          mPlugin->setParameterAutomated(par->mID,val); // will call back to onChange below (to be redrawn)
        }
        else axWindow::onChange(aWidget);//redrawWidget(aWidget);
      }

    //----------

    virtual void onChange(axParameter* aParameter)
      {
        //TRACE("- axEditor: onChange(par)... par=%x\n",(int)aParameter);
        int con = aParameter->mCNum;
        //TRACE("par.con: %i\n",con);
        if( con>=0 )
        {
          axWidget* wdg = mConnections[con]->mWidget;
          //TRACE("  wdg=%x\n",(int)wdg);
          float val = aParameter->doGetValue();
          //wdg->doSetValue(val);
          wdg->mValue = val;
          //redrawWidget( wdg );
          appendDirty(wdg);
        }
      }

    void resizeWindow(int aWidth, int aHeight)
      {
        mRect.w = aWidth;
        mRect.h = aHeight;
        //resizeBuffer(aWidth,aHeight);
        //doRealign();
        setSize( aWidth, aHeight );             // resize os window

        mPlugin->mWidth = aWidth;
        mPlugin->mHeight = aHeight;
        mPlugin->sizeWindow(aWidth, aHeight);  // let vst host know (request to resize window), Expose
      }

    //virtual void onCursor(int aCursor)
    //  {
    //    TRACE("set cursor %i\n",aCursor);
    //    setCursor(aCursor);
    //  }


};

//----------------------------------------------------------------------
#endif


