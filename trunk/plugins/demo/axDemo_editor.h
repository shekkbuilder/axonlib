#ifndef axDemo_editor_included
#define axDemo_editor_included
//----------------------------------------------------------------------

#include "axEditor.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgPages.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgButtons.h"

//#include "axDemo_skin.h"

#include "axDemo_page_canvas.h"
#include "axDemo_page_widgets.h"

//----------------------------------------------------------------------

class axDemo_editor : public axEditor
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    wdgPanel*   w_LeftPanel;
    wdgSizer*   w_Sizer;
    wdgPages*   w_RightPanel;
    wdgPanel*   w_Status;
    wdgButtons* w_PageSelect;

    axDemo_page_canvas*   w_Page_canvas;
    axDemo_page_widgets*  w_Page_widgets;

  public:
    axDemo_editor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {
        //setSkin(mDefaultSkin);

        //----- left side, the page selectors

        appendWidget( w_LeftPanel = new wdgPanel(this,axRect(0,0,100,0),wa_Left) );
          w_LeftPanel->appendWidget( w_PageSelect = new wdgButtons(this,NULL_RECT,wa_Client) );
            w_PageSelect->setBorders(5,5,2,2);
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,0,80,20),wa_Top,false,"canvas", "[ canvas ]", ta_Center,bm_Switch) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,0,80,20),wa_Top,false,"widgets","[ widgets ]",ta_Center,bm_Switch) );
            w_PageSelect->setMode(bm_Single);

        //----- middle resize bar

        appendWidget( w_Sizer = new wdgSizer(this,axRect(0,0,5,0),wa_Left) );
          w_Sizer->setTarget(w_LeftPanel);

        //----- bottom, status bar

        appendWidget( w_Status = new wdgPanel(this,axRect(0,0,0,20),wa_Bottom) );

        //----- right, pages (aka tabs)

        appendWidget( w_RightPanel = new wdgPages(this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_canvas = new axDemo_page_canvas(this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_widgets = new axDemo_page_widgets(this,NULL_RECT,wa_Client) );
            w_Page_widgets->setBorders(5,5,2,2);
            w_Page_widgets->setupWidgets(this);
          w_RightPanel->setPage(0,false);

        //----- and put everything in place

        doRealign();

      }

    //----------

    virtual ~axDemo_editor()
      {
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==w_PageSelect)
        {
          int id = w_PageSelect->getValue();
          w_RightPanel->setPage(id,true);
        }
        //plugin->changedParam
        axEditor::onChange(aWidget);
      }

    //--------------------------------------------------

    virtual void doTimer(void)
      {
        trace("doTimer");
      }

};

//----------------------------------------------------------------------
#endif
