//#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_PNG
#define AX_DEBUG_NEW
#define AX_DEBUG_LOG "test.log"

//#define AX_NOGUI
#define AX_USE_CPU_CAPS

//----------

#include "base/axBase.h"
#include "core/axCpu.h"
#include "gui/axWindow.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgKnob.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) {}
    //virtual ~myDescriptor() {}
    virtual bool    hasEditor(void)     { return true; }
    virtual axRect  getEditorRect(void) { return axRect(0,0,320,240); }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
    axBase*       mBase;
    axRect        mEditorRect;
    axEditor*     edit;
    axSkinBasic*  skin;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        mBase = aBase;
        edit  = NULL;
        skin  = NULL;
        test();
      }

    //----------

    //virtual ~myInstance()
    //  {
    //  }

    //--------------------------------------------------

    void test(void)
      {
        trace(AX_AXONLIB_TEXT);
        char temp[256];
        trace("basepath:        '" << axGetBasePath(temp) << "'");
        trace("platform:        '" << mBase->getPlatform()->getPlatformName() << "'");
        trace("format:          '" << mBase->getFormat()->getFormatName() << "'");
        trace("w32 class name   '" << mBase->getInterface()->getName() << "'");
        axCpu cpu;
        cpu.axCpuId();
        unsigned long long start_time, end_time, diff;
        start_time = cpu.rdtsc();
        end_time   = cpu.rdtsc();
        diff       = end_time - start_time;
        //trace("axCpuCaps:        " << cpu.axCpuCaps() );
        trace("cpu caps          " << cpu.axCpuCaps() << " '" << cpu.axCpuCapsString() << "'" );
        trace("rdtsc:            " << diff << " (may not be correct for multi-core)");
      }

    //--------------------------------------------------

    //virtual void doStateChange(int aState)
    //  {
    //    trace("doStateChange: " << aState);
    //  }

    //--------------------------------------------------

    virtual void* doOpenEditor(void* ptr)
      {
        edit = (axEditor*)mBase->getInterface()->createEditor(ptr,getEditorRect(),AX_WIN_DEFAULT);
        if (edit)
        {
          edit->show();
          //win->show();
          axCanvas* canvas = edit->getCanvas();
          skin = new axSkinBasic(canvas);
          edit->applySkin(skin);
          edit->setInstance(this);
          wdgPanel* panel;
          edit->appendWidget( panel = new wdgPanel(edit,NULL_RECT,wa_Client));
            panel->appendWidget( new wdgButton(edit,axRect(10,10,100,30),wa_None));
            panel->appendWidget( new wdgKnob(  edit,axRect(10,50,100,30),wa_None));
          edit->doRealign();
        }
        return edit;
      }

    //----------

    virtual void  doCloseEditor(void)
      {
        if (skin) { delete skin; skin=NULL; }
        if (edit) { delete edit; edit=NULL; }
      }

    //

};

//----------------------------------------------------------------------

//AX_MAIN(myPlugin)
AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
