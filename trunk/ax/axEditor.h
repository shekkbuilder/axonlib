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
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------
// mainly does two things:
// - onChange(wdg/par) transfer mValue, widget <-> parameter (if mID>0)
// - keep track of 'dirty widgets'

//#define AX_DIRTYWIDGETS

#include "axParameter.h"
#include "axPlugin.h"
#include "axWindow.h"

//#include "axMutex.h"

//----------------------------------------------------------------------

// widget <-> parameter connection

//class axWPConnection
struct axWPConnection
{
  //public:
    axWidget    *mWidget;
    axParameter *mParameter;
  //public:
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
// editor
//
//----------------------------------------------------------------------

/// plugin editor
/**
  the editor does mainly three things:
  - keep track of a dirty widgets list
  - keep track of connections between widgets and parameters
  - can resize plugin window
*/

class axEditor : public axWindow,
                 public axParameterListener
{
  private:
    //axMutex   mutex_dirty;
  protected://public:
    axPlugin        *mPlugin;
    axWPConnections mConnections;
    #ifdef AX_DIRTYWIDGETS
    axWidgets       mDirtyList;
    #endif

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

    #ifdef AX_DIRTYWIDGETS

    /// clear list of dirty widgets

    void clearDirty(void)
      {
        //mutex_dirty.lock();
        mDirtyList.clear(false);
        //mutex_dirty.unlock();
      }

    //----------

    /// append to dirty-widgets list
    /**
      if it isn't already in the list
    */

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

    /// paint all widgets in dirty-widgets list

    void redrawDirty(void)
      {
        //mutex_dirty.lock();
        int num = mDirtyList.size();
        for( int i=0; i<num; i++ )
        {
          axWidget* wdg = mDirtyList[i];
          redrawWidget(wdg);
        }
        clearDirty();
        //mutex_dirty.unlock();
        //flush();

      }

    #endif

    //----------------------------------------
    // connections
    //----------------------------------------

    /// connect widget to parameter
    /**
      for automatic updating of values from one to the another
    */

    void connect(axWidget* aWidget, axParameter* aParameter)
      {
        //aWidget->mParameter = aParameter;
        aWidget->setParameter(aParameter);
        int num = mConnections.size();
        aWidget->setConnectNum(num);
        aParameter->setConnectNum(num);
        //aParameter->connection(num);
        aWidget->doSetValue( aParameter->doGetValue() );
        //lock
        mConnections.append( new axWPConnection(aWidget,aParameter) );
        //unlock
      }

    //----------

    /// todo

    void connectAll(void)
      {
      }

    //----------

    /// delete & clear the connections list

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

    /// called from gui when tweaking widget

    virtual void onChange(axWidget* aWidget)
      {
        //TRACE("- axEditor: onChange(wdg)... id=%i\n",aWidget->mID);
        int con = aWidget->getConnectNum();
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

    /// called when parameter changed
    /**
      by default, if there is a connection between this parameter, and a widget, the widget is notified (doSetValue).
      then, either appended to the dirty widgets list if AX_DIRTYWIDGETS is defined, or if not, called for redrawing (redrawWidget)
      \param aParameter the parameter that has just changed
    */

    virtual void onChange(axParameter* aParameter)
      {
        int con = aParameter->getConnectNum();
        if( con>=0 )
        {
          axWidget* wdg = mConnections[con]->mWidget;
          float val = aParameter->doGetValue();
          wdg->doSetValue(val);
          #ifdef AX_DIRTYWIDGETS
          appendDirty(wdg);
          #else
          redrawWidget(wdg);
          #endif
        }
      }

    /// resize window
    /**
      this one needs more work. specific hosts might need specific actions...
      calls axWIndow.setSize, set size of rect that will be returned in effEditGetRect, and the vst function sizeWindow is called.
      various hosts react differently to these, and we need to do a host-check at startup, and do more testing to see which host require what.
      \param aWidth new width of plugin window
      \param aHeight new height
    */

    void resizeWindow(int aWidth, int aHeight)
      {
        mRect.w = aWidth;
        mRect.h = aHeight;
        setSize(aWidth,aHeight);  // resize os window
        //resizeBuffer(aWidth,aHeight);
        //doRealign();
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


