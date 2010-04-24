#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------

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

  public:

    axEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axWindow(aContext,aRect,aWinFlags)
      {
        mPlugin = aPlugin;
        axCanvas* can = getCanvas();
        mDefaultSkin = new axSkinDefault(can);
        applySkin(mDefaultSkin);
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


    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        if (mPlugin) mPlugin->notifyResizeEditor(aWidth,aHeight);
        axWindow::doSetSize(aWidth,aHeight);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        //wtrace("axEditor.onChange");
        int conn = aWidget->getConnection();
        //wtrace("  conn = " << conn);
        if (conn>=0)
        {
          axParameter* par = mConnections[conn].mParameter;
          //wtrace("  par = " << par);
          float val = aWidget->getValue();
          //wtrace("  val = " << val);
          //wtrace("  mPlugin = " << mPlugin);
          mPlugin->notifyParamChanged(par);
          par->doSetValue(val);
          #ifdef AX_FORMAT_EXE
            redrawWidget(aWidget);
          #endif
        }
        else
        {
          redrawWidget(aWidget);
        }
      }

    //TODO: dirtyWidgets

    //virtual void onRedraw(axWidget* aWidget) { redrawWidget(aWidget); }
    //virtual void onCursor(int aCursor) { setCursor(aCursor); }
    //virtual void onHint(axString aHint) {}
    //virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY)

    //----------


};

//----------------------------------------------------------------------
#endif

