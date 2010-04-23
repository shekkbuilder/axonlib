#define AX_DEBUG
#define AX_DEBUG_AUTO_STD

#include "axPlugin.h"
#include "axDemo_editor.h"
#include "axDemo_graph.h"

class axDemo : public axPlugin
{
  private:
    axDemo_editor* mEditor;
    axParameter   *p1,*p2,*p3,*p4,*p5;
  public:

    axDemo(axContext* aContext)
    : axPlugin(aContext, pf_HasEditor)
      {
        describe("axDemo","ccernn","axonlib example",0,AX_MAGIC+0xFFFF);
        setupAudio(2,2,false);
        setupEditor(640,480);
        appendParameter( p1 = new axParameter(this,"p1","") );
        appendParameter( p2 = new axParameter(this,"p2","") );
        appendParameter( p3 = new axParameter(this,"p3","") );
        appendParameter( p4 = new axParameter(this,"p4","") );
        appendParameter( p5 = new axParameter(this,"p5","") );
        setupParameters();
      }

    virtual ~axDemo()
      {
      }

    //---------- plugin ----------

    virtual void  doStateChange(int aState)
      {
        trace(":: doStateChange " << aState);
      }

    virtual void  doTransportChange(int aState)
      {
        trace(":: doTransportChange " << aState);
      }

    virtual void  doSetProgram(int aProgram)
      {
        trace(":: doSetProgram " << aProgram);
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        char buf[32];
        aParameter->doGetDisplay(buf);
        trace(":: doSetParameter " << aParameter->getName().ptr() << " = " << aParameter->getValue() << " (" << buf << ")");
      }

    //virtual bool  doProcessEvents(void) { return false; }

    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        trace(":: doProcessMidi "<<ofs<<" : "<<(int)msg1<<","<<(int)msg2<<","<<(int)msg3);
      }

    //virtual bool  doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
    //  {
    //    return false;
    //  }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        //*aOutputs[0] = *aInputs[0];
        //*aOutputs[1] = *aInputs[0];
        *aOutputs[0] = axRandomSigned() * 0.5;
        *aOutputs[1] = axRandomSigned() * 0.5;
      }

    //virtual void  doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize)
    //  {
    //  }

    //---------- editor ----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        //wtrace(":: doOpenEditor");
        mEditor = new axDemo_editor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->connect(mEditor->w_Page_widgets->w1,p1);
        mEditor->connect(mEditor->w_Page_widgets->w2,p2);
        mEditor->connect(mEditor->w_Page_widgets->w3,p3);
        mEditor->connect(mEditor->w_Page_widgets->w4,p4);
        mEditor->connect(mEditor->w_Page_widgets->w5,p5);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        //wtrace(":: doCloseEditor");
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //----------

    virtual void doIdleEditor()
      {
        //trace("doIdleEditor");
      }

};


//----------------------------------------------------------------------
AX_ENTRYPOINT(axDemo)
