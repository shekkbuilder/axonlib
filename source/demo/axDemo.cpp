//#define AX_NO_MALLOC
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_LOG        "axDemo.cpp.log"

#define AX_ALPHA

#include "format/axFormat.h"
#include "core/axRand.h"
#include "gui/axBitmapLoader.h"
#include "axDemo_editor.h"
#include "axDemo_graph.h"
//#include "demoskin.h"
//#include "demoknob.h"

//#include "audio/axLibaam.h"

class axDemo : public axFormat
{
  private:
    axDemo_editor* m_Editor;
    axDemo_graph*  m_Graph;
    axParameter   *p1,*p2,*p3,*p4,*p5;
    bool m_GuiInitialized;
    axBitmapLoader* skinloader;
    axBitmapLoader* knobloader;
  public:

    axDemo(axContext* aContext, int aFormatFlags)
    : axFormat(aContext)
      {
        m_GuiInitialized = false;
        m_Graph = new axDemo_graph("axDemo graph");
        m_Graph->doCompile();
        describe("axDemo","ccernn","axonlib example",0,AX_MAGIC+0xFFFF);
        setupAudio(2,2,false);
        setupEditor(500,500);
        appendParameter( p1 = new axParameter(this,"p1","",0.2) );
        appendParameter( p2 = new axParameter(this,"p2","",0.4) );
        appendParameter( p3 = new axParameter(this,"p3","",0.6) );
        appendParameter( p4 = new axParameter(this,"p4","",0.8) );
        appendParameter( p5 = new axParameter(this,"p5","",1.0) );
        setupParameters();
        //char* caps = axCpuCapsString();
        //trace("cpu caps: " << caps);

//  char buffer[17];
//  axFtoa(buffer,PI);
//  trace("axFtoa(PI) = '" << buffer << "'");
//  axItoa(buffer,418);
//  trace("axItoa(418) = '" << buffer << "'");
//  if (axBigEndian()) trace("big endian");
//  if (axLittleEndian()) trace("little endian");

      }

    //----------

    virtual ~axDemo()
      {
        delete m_Graph;
        if (m_GuiInitialized)
        {
          delete skinloader;
          delete knobloader;
        }
      }

    //---------- plugin ----------

    virtual void  doStateChange(int aState)
      {
        trace(":: doStateChange " << aState);
      }

    //----------

    virtual void  doTransportChange(int aState)
      {
        trace(":: doTransportChange " << aState);
      }

    //----------

    virtual void  doSetProgram(int aProgram)
      {
        trace(":: doSetProgram " << aProgram);
      }

    //---------- param/midi ----------

    virtual void  doSetParameter(axParameter* aParameter)
      {
        char buf[32];
        aParameter->doGetDisplay(buf);
        trace(":: doSetParameter " << aParameter->getName().ptr() << " = " << aParameter->getValue() << " (" << buf << ")");
        //todo: send param to graph
        if (aParameter==p5)
        {
          //mGraph->param = p5->doGetValue();
        }
      }

    //----------

    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        trace(":: doProcessMidi "<<ofs<<" : "<<(int)msg1<<","<<(int)msg2<<","<<(int)msg3);
        //todo: send midi to graph
        int msg = (msg1 & 0xf0) >> 4;
        if (msg==9)
        {
          //if (msg3>0) mGraph->gate->doSignal(2,st_Gate,1);
          //else mGraph->gate->doSignal(2,st_Gate,0);
        }
        //else if (msg==8) mGraph->gate->doSignal(2,st_Gate,0);
      }

    //---------- audio ----------

    virtual bool  doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        updateTimeInfo();
        if (m_Editor)
        {
          m_Editor->w_Page_sync->update( getPlayState(), getSamplePos(), getSampleRate(), getBeatPos(), getTempo() );
        }
        return false;
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        //*aOutputs[0] = *aInputs[0];
        //*aOutputs[1] = *aInputs[0];
        *aOutputs[0] = axRandSigned() * 0.1f;
        *aOutputs[1] = axRandSigned() * 0.1f;
        //todo: graph.process
        //mGraph->doExecute(aInputs,aOutputs);

      }

    //----------

    //virtual void  doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize)
    //  {
    //  }

    //---------- editor ----------

// #define AX_WIN_DEFAULT ( AX_WIN_BUFFERED
//                        | AX_WIN_MSGTHREAD
//                        | AX_WIN_EMBEDDED)

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        trace(":: doOpenEditor");
        //if (!m_GuiInitialized)
        //{
        //  skinloader = new axBitmapLoader();
        //  skinloader->decodePng((unsigned char*)demoskin,demoskin_size);
        //  knobloader = new axBitmapLoader();
        //  knobloader->decodePng((unsigned char*)demoknob,demoknob_size);
        //  m_GuiInitialized = true;
        //}
        m_Editor = new axDemo_editor(this,aContext,mEditorRect,AX_WIN_DEFAULT/*AX_WIN_MSGTHREAD | AX_WIN_MSGDELETE*/ );
        //m_Editor->setupSkin(skinloader,knobloader);
        m_Editor->setup(getSystemInfo(),getHostInfo());
        m_Editor->connect(m_Editor->w_Page_widgets->w1,p1);
        m_Editor->connect(m_Editor->w_Page_widgets->w2,p2);
        m_Editor->connect(m_Editor->w_Page_widgets->w3,p3);
        m_Editor->connect(m_Editor->w_Page_widgets->w4,p4);
        m_Editor->connect(m_Editor->w_Page_widgets->w5,p5);
        m_Editor->show();
        //mEditor->startTimer(500);
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        trace(":: doCloseEditor");
        //mEditor->stopTimer();
        axEditor* editor = m_Editor;
        editor->hide();
        m_Editor = NULL;
        delete editor;
      }

    //----------

    //this should be in axEditor? (axFormat...)

    virtual void doIdleEditor()
      {
        #ifndef AX_WIDGET_NOUPDATELIST
        if (m_Editor) m_Editor->redrawUpdates();
        #endif
        //trace("doIdleEditor");
      }

    //--------------------------------------------------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(axDemo)
