#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------

#include "axPlugin.h"
#include "gui/axWindow.h"

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
    axSkinDefault*  mDefaultSkin;
    wp_connections  mConnections;

  public:

    axEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axWindow(aContext,aRect,aWinFlags)
      {
        mPlugin = aPlugin;
        axCanvas* can = getCanvas();
        mDefaultSkin = new axSkinDefault(can);
        doSetSkin(mDefaultSkin);
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

    virtual void onChange(axWidget* aWidget)
      {
        int conn = aWidget->getConnection();
        if (conn>=0)
        {
          axParameter* par = mConnections[conn].mParameter;
          float val = aWidget->getValue();
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

    //----------

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

};

//----------------------------------------------------------------------
#endif
