//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test.log"

//#define AX_NOGUI
#define AX_USE_CPU_CAPS

//----------

#include "core/axCpu.h"
#include "base/axBase.h"
#include "gui/axWindow.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
//#include "wdg/wdgPanel.h"
#include "wdg/wdgButton.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { /*trace("myDescriptor.constructor");*/ }
    virtual ~myDescriptor() { /*trace("myDescriptor.destructor");*/ }
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
    //axWindow*     win;
    axEditor*     edit;
    axSkinBasic*  skin;
    char temp[256];

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //trace("myInstance.constructor");
        mBase = aBase;
        //win = NULL;
        edit = NULL;
        skin = NULL;

        trace("platform:          '" << aBase->getPlatform()->getPlatformName() << "'");
        // we can't access the mFormat class in the constructor/destructor of axInstance or axDescriptor
        // since these classes are created in the constructor  of axFormatExe/Vst/..
        trace("format:            '" << aBase->getFormat()/*->getFormatName() << "'"*/);   // NULL ???
        trace("format:            '" << aBase->getFormat()->getFormatName() << "'");   // NULL ???
        trace("axGetBasePath:     '" << axGetBasePath(temp) << "'");
        trace("interface.getName: '" << aBase->getInterface()->getName() << "'");
        trace("AX_AXONLIB_TEXT:   '" << AX_AXONLIB_TEXT << "'");

        axCpu cpu;
        cpu.axCpuId();
        unsigned long long start_time, end_time, diff;
        start_time = cpu.rdtsc(); // may not work correctly for multi-core
        end_time   = cpu.rdtsc();
        diff       = end_time - start_time;

        trace("axCpuCaps:          " << cpu.axCpuCaps() );
        trace("axCpuCapsString:   '" << cpu.axCpuCapsString() << "'");
        trace("rdtsc:              " << diff << " (may not be correct for multi-core)");

        //setupEditor(320,240);
        trace("MyInstace ok");
      }

    //----------

    //virtual ~myInstance()
    //  {
    //    /*trace("myInstance.destructor");*/
    //  }

    //--------------------------------------------------

//    virtual void setupEditor(int aWidth, int aHeight)
//      {
//        mEditorRect.set(0,0,aWidth,aHeight);
//      }
//
//    virtual axRect getEditorRect(void) { return mEditorRect; }

    //--------------------------------------------------

    virtual void* doOpenEditor(void* ptr)
      {
        trace("doOpenEditor");
        //win = (axWindow*)mBase->getInterface()->createWindow(ptr,axRect(0,0,320,240),AX_WIN_DEFAULT);
        //edit = (axEditor*)mBase->getInterface()->createWindow(ptr,axRect(0,0,320,240),AX_WIN_DEFAULT);
        edit = (axEditor*)mBase->getInterface()->createEditor(ptr, getEditorRect()/*axRect(0,0,320,240)*/,AX_WIN_DEFAULT);
        if (edit)
        {
          edit->show();
          //win->show();
          axCanvas* canvas = edit->getCanvas();
          skin = new axSkinBasic(canvas);
          edit->applySkin(skin);
          edit->setInstance(this);
          //win->appendWidget( new wdgPanel(win,NULL_RECT,wa_Client));
          edit->appendWidget( new wdgButton(edit,NULL_RECT,wa_Client));
          edit->doRealign();
        }
        return edit;
      }

    //----------

    virtual void  doCloseEditor(void)
      {
        if (skin) delete skin;
        skin = NULL;
        if (edit) delete edit;
        edit = NULL;
      }

};

//----------------------------------------------------------------------

//AX_MAIN(myPlugin)
AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
