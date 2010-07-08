//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test.log"

//#define AX_NOGUI
#include "base/axBase.h"
#include "gui/axWindow.h"
#include "skins/axSkinBasic.h"
#include "wdg/wdgPanel.h"
//----------------------------------------------------------------------

//class myPlugin : public AX_FORMAT
//{
//  public:
//    myPlugin(axBase* aBase) : AX_FORMAT(aBase)
//      {
//        trace("- myPlugin.constructor");
//      }
//};

//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { /*trace("myDescriptor.constructor");*/ }
    virtual ~myDescriptor() { /*trace("myDescriptor.destructor");*/ }
};

//----------

class myInstance : public AX_INSTANCE
{
  private:
    axWindow*     win;
    axSkinBasic*  skin;

  public:
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //trace("myInstance.constructor");
        //char temp[256];
        //trace( axGetBasePath(temp) );
        win = (axWindow*)aBase->getInterface()->createWindow(NULL,axRect(0,0,320,240),AX_WIN_DEFAULT);
        win->show();

        axCanvas* canvas = win->getCanvas();
        skin = new axSkinBasic(canvas);
        win->applySkin(skin);

        win->appendWidget( new wdgPanel(win,NULL_RECT,wa_Client));
        win->doRealign();

// test
        win->eventLoop();
        win->hide();
        delete win;
        delete skin;
// ----
      }
    virtual ~myInstance()
      {
        /*trace("myInstance.destructor");*/
      }
};

//----------------------------------------------------------------------

//AX_MAIN(myPlugin)

AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
