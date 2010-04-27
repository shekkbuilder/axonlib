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
  friend class axDemo;
  private:
    wdgPanel*             w_LeftPanel;
    wdgPages*             w_RightPanel;
    wdgSizer*             w_Sizer;
    wdgSizer*             w_WinSizer;
    wdgPanel*             w_Status;
    wdgScrollBox*         w_Scroll;
    wdgButtons*           w_Select;

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

        appendWidget( w_LeftPanel = new wdgPanel(this,axRect(0,0,150,0),wa_Left) );

        w_LeftPanel->appendWidget( w_Scroll = new wdgScrollBox(this,NULL_RECT,wa_Client) );

          w_Scroll->appendWidget( w_Select = new wdgButtons(this,axRect(0,180+26),wa_Top) );
            w_Select->getContainer()->setBorders(5,5,2,2);
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"canvas", "canvas" ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"widgets","widgets") );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"system", "system" ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"host",   "host"   ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"sync",   "sync"   ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"params", "params" ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"audio",  "audio"  ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"midi",   "midi"   ) );
            w_Select->appendButton(new wdgButton(w_Select,axRect(0,20),wa_Top,false,"bitmaps","bitmaps") );
            w_Select->setMode(bm_Single);

        //----- [resizer] -----

        appendWidget( w_Sizer = new wdgSizer(this,axRect(5,0),wa_Left) );
          w_Sizer->setTarget(w_LeftPanel);

        //----- bottom [status panel] -----

        appendWidget( w_Status = new wdgPanel(this,axRect(0,20),wa_Bottom) );
          w_Status->appendWidget( w_WinSizer = new wdgSizer(this,axRect(10,10),wa_RightBottom) );
            //w_WinSizer->setTarget(this);

        //----- right [pages, aka tabs] -----

        appendWidget( w_RightPanel = new wdgPages(this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_canvas  = new axDemo_page_canvas( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_widgets = new axDemo_page_widgets(this,NULL_RECT,wa_Client) );
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
        startTimer(1000);
      }

    //----------

    virtual ~axDemo_editor()
      {
        stopTimer();
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==w_Select)
        {
          //trace("axDemo_editor.onChange");
          int id = w_Select->getVal();
          //resizeWindow(100+id*100,300);
          w_RightPanel->setPage(id,true); // redraw

        }
        //todo: plugin.param
        axEditor::onChange(aWidget);
      }

    //--------------------------------------------------

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY)
      {
        if (aWidget==w_WinSizer)
        {
          {
            trace("onSize " << aDeltaX << "," << aDeltaY);
            axRect R = mPlugin->getEditorRect();
            trace("       " << R.w << "," << R.h);
            //trace("       " << mRect.w << "," << mRect.h);
            resizeWindow( R.w + aDeltaX, R.h + aDeltaY );
            //axWindow::onSize(aWidget, aDeltaX, aDeltaY);
          }
        }
        else
        axEditor::onSize(aWidget, aDeltaX, aDeltaY);
        //int w = mRect.w + aDeltaX;
        //int h = mRect.h + aDeltaY;
        //axWidget::doSetSize(w,h);
        //mListener->onSize(aWidget,aDeltaX,aDeltaY);
      }


    //--------------------------------------------------

    virtual void doTimer(void)
      {
        trace("doTimer");
      }

};

//----------------------------------------------------------------------
#endif
