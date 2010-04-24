#ifndef axDemo_editor_included
#define axDemo_editor_included
//----------------------------------------------------------------------

#include "axEditor.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgPages.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgButtons.h"
#include "wdg/wdgScrollBox.h"

//#include "axDemo_skin.h"

#include "axDemo_page_canvas.h"
#include "axDemo_page_widgets.h"
#include "axDemo_page_system.h"
#include "axDemo_page_host.h"
#include "axDemo_page_sync.h"
#include "axDemo_page_params.h"
#include "axDemo_page_audio.h"
#include "axDemo_page_midi.h"
#include "axDemo_page_bitmaps.h"

//----------------------------------------------------------------------

class axDemo_editor : public axEditor
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    wdgPanel*   w_LeftPanel;
    //wdgScrollBox* w_LeftPanel;
    wdgSizer*     w_Sizer;
    wdgPages*     w_RightPanel;
    //wdgScrollBox* w_RightPanel;
    wdgPanel*     w_Status;
    wdgButtons*   w_PageSelect;

    axDemo_page_canvas*   w_Page_canvas;
    axDemo_page_widgets*  w_Page_widgets;
    axDemo_page_system*   w_Page_system;
    axDemo_page_host*     w_Page_host;
    axDemo_page_sync*     w_Page_sync;
    axDemo_page_params*   w_Page_params;
    axDemo_page_audio*    w_Page_audio;
    axDemo_page_midi*     w_Page_midi;
    axDemo_page_bitmaps*  w_Page_bitmaps;

  public:

    axDemo_editor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {

        //----- left [page Buttons] -----

        //appendWidget( w_LeftPanel = new wdgScrollBox(this,axRect(0,0,150,0),wa_Left) );
        //appendWidget( w_PageSelect = new wdgButtons(this,axRect(0,0,150,0),wa_Left) );
        appendWidget( w_LeftPanel = new wdgPanel(this,axRect(0,0,150,0),wa_Left) );
          w_LeftPanel->appendWidget( w_PageSelect = new wdgButtons(this,axRect(0,180+8*5),wa_Top) ); // mContent not done in wdgButtons, so we hardcode it
            w_PageSelect->setBorders(5,5,0,0);
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"canvas", "canvas" ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"widgets","widgets") );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"system", "system" ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"host",   "host"   ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"sync",   "sync"   ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"params", "params" ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"audio",  "audio"  ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"midi",   "midi"   ) );
            w_PageSelect->appendButton(new wdgButton(w_PageSelect,axRect(0,20),wa_Top,false,"bitmaps","bitmaps") );
            w_PageSelect->setMode(bm_Single);

        //----- [resizer] -----

        appendWidget( w_Sizer = new wdgSizer(this,axRect(0,0,5,0),wa_Left) );
          w_Sizer->setTarget(w_LeftPanel);

        //----- bottom [status panel] -----

        appendWidget( w_Status = new wdgPanel(this,axRect(0,0,0,20),wa_Bottom) );

        //----- right [pages, aka tabs] -----

        appendWidget( w_RightPanel = new wdgPages(this,NULL_RECT,wa_Client) );
        appendWidget( w_RightPanel = new wdgPages(this,NULL_RECT,wa_Client) );

          w_RightPanel->appendPage( w_Page_canvas  = new axDemo_page_canvas( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_widgets = new axDemo_page_widgets(this,NULL_RECT,wa_Client) );
            w_Page_widgets->setBorders(5,5,2,2);
            w_Page_widgets->setupWidgets(this);
          w_RightPanel->appendPage( w_Page_system  = new axDemo_page_system( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_host    = new axDemo_page_host(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_sync    = new axDemo_page_sync(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_params  = new axDemo_page_params( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_audio   = new axDemo_page_audio(  this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_midi    = new axDemo_page_midi(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_bitmaps = new axDemo_page_bitmaps(this,NULL_RECT,wa_Client) );
          w_RightPanel->setPage(0,false);

        //----- and then put everything in place -----

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
        //todo: plugin.param
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
