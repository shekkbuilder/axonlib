#ifndef axDemo_editor_included
#define axDemo_editor_included
//----------------------------------------------------------------------

/*
  this one has the layout of the gui, the pages/tabs, status bar,
  etc.
*/

#include "axEditor.h"
#include "gui/axSymbols.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgPages.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgButtons.h"
#include "wdg/wdgScrollBox.h"

#include "axDemo_skin.h"
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

unsigned char demo_skin[] = {};
int           demo_skin_size = 0;

//----------------------------------------------------------------------

class axDemo_editor : public axEditor
{
  friend class axDemo;
  private:
    axDemo_skin*          m_Skin;
    axBitmap*             m_VoxelBitmap;
    axSymbols*            m_Symbols;

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

        axCanvas* canvas = getCanvas();
        m_Skin = new axDemo_skin(canvas);
        applySkin(m_Skin);
        //m_Skin->loadSkinBitmap(this,(unsigned char*)demo_skin,demo_skin_size);
        //m_Symbols = new axSymbols(m_Skin->getSurface());
        m_VoxelBitmap = createBitmap(320,200);
        m_VoxelBitmap->createBuffer();
        m_VoxelBitmap->prepare();

        //----- bottom [status panel] -----

        appendWidget( w_Status = new wdgPanel(this,axRect(0,20),wa_Bottom) );
          w_Status->appendWidget( w_WinSizer = new wdgSizer(this,axRect(10,10),wa_RightBottom,sm_Window) );
          //w_WinSizer->setTarget(this);

        //----- left [page Buttons] -----

        appendWidget( w_LeftPanel = new wdgPanel(this,axRect(0,0,100,0),wa_Left) );
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

        appendWidget( w_Sizer = new wdgSizer(this,axRect(5,0),wa_Left,sm_Horizontal) );
          w_Sizer->setTarget(w_LeftPanel);

        //----- right [pages, aka tabs] -----

        appendWidget( w_RightPanel = new wdgPages(this,NULL_RECT,wa_Client) );
        //w_RightPanel->setLimits(100,100);//,9999,9999);
          w_RightPanel->appendPage( w_Page_canvas  = new axDemo_page_canvas( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_widgets = new axDemo_page_widgets(this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_system  = new axDemo_page_system( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_host    = new axDemo_page_host(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_sync    = new axDemo_page_sync(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_params  = new axDemo_page_params( this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_audio   = new axDemo_page_audio(  this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_midi    = new axDemo_page_midi(   this,NULL_RECT,wa_Client) );
          w_RightPanel->appendPage( w_Page_bitmaps = new axDemo_page_bitmaps(this,NULL_RECT,wa_Client,m_VoxelBitmap) );
          w_RightPanel->setPage(0,false);

        //----- and then put everything in place -----

        doRealign();
        startTimer(50);
      }

    //----------

    virtual ~axDemo_editor()
      {
        stopTimer();
        delete m_Symbols;
        delete m_VoxelBitmap;
        delete m_Skin;
      }

    //--------------------------------------------------

    void setup(axSystemInfo* aSystemInfo, axHostInfo* aHostInfo)
      {
        w_Page_system->setup(aSystemInfo);
        w_Page_host->setup(aHostInfo);
      }

    //--------------------------------------------------
    // on
    //--------------------------------------------------

    // change page
    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==w_Select)
        {
          int id = w_Select->getVal();
          w_RightPanel->setPage(id,true);
        }
        //todo: plugin.param
        axEditor::onChange(aWidget);
      }

    //--------------------------------------------------
    // do
    //--------------------------------------------------

    // send timer tick to our bitmap page
    // (the voxel landscape)
    virtual void doTimer(void)
      {
        w_Page_bitmaps->timer_tick();
      }

    //----------

    // optional:
    // close modal popup windiw when a key is pressed
    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) unModal();
        else axEditor::doKeyDown(aKeyCode,aState);
      }

    //----------

    // optional:
    // close modal popup windiw when a right mouse button is pressed
    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget && (aButton==bu_Right)) unModal();
        else axEditor::doMouseDown(aXpos,aYpos,aButton);
      }

};

//----------------------------------------------------------------------
#endif
