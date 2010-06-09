//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#include "format/axFormat.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
#include "gui/axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgSlider.h"

#include <math.h>

//---------------------------------------------------------------------

#define NUMTYPES 9

char* str_type1[] =
{
  (char*)"sinf",
  (char*)"cosf",
  (char*)"expf",
  (char*)"logf",
  (char*)"floorf",
  (char*)"ceilf",
  (char*)"roundf",
  (char*)"abs",
  (char*)"signbit"
};

char* str_type2[] =
{
  (char*)"axSinf",
  (char*)"axCosf",
  (char*)"axExpf",
  (char*)"axLog",
  (char*)"axFloor",
  (char*)"axCeil",
  (char*)"axRound",
  (char*)"axAbs",
  (char*)"axSign"
};

float calc1(int type, float n)
  {
    switch (type)
    {
      case 0: return sinf(n);
      case 1: return cosf(n);
      case 2: return expf(n);
      case 3: return logf(n);
      case 4: return floorf(n);
      case 5: return ceilf(n);
      case 6: return roundf(n);
      case 7: return fabs(n);
      case 8: return signbit(n);//sign(n);
    }
    return 0;
  }

float calc2(int type, float n)
  {
    switch (type)
    {
      case 0: return axSinf(n);
      case 1: return axCosf(n);
      case 2: return axExpf(n);
      case 3: return axLog(n);
      case 4: return axFloor(n);
      case 5: return axCeil(n);
      case 6: return axRound(n);
      case 7: return axAbs(n);
      case 8: return axSign(n);
    }
    return 0;
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPainter : public axWidget
{
  public:
    int type1,type2;
    float minx,miny,maxx,maxy;
    float nval;

  public:

    myPainter(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
      }

    //----------


    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        // background

        aCanvas->setBrushColor( aCanvas->getColor(AX_GREY_DARK) );
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );

        aCanvas->setClipRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());

        float rangex = maxx-minx;
        float rangey = maxy-miny;
        float scalex = 0;
        float scaley = 0;
        float x0 = 0;
        float y0 = 0;

        // x axis

        aCanvas->setPenColor( aCanvas->getColor(AX_GREY) );
        if ((minx<0) && (rangex>0))
        {
          scalex = mRect.w / rangex;
          x0 = mRect.x + axAbs(minx) * scalex;
          aCanvas->drawLine(x0,mRect.y,x0,mRect.y2());
        }

        // y axis

        if ((miny<0) && (rangey>0))
        {
          scaley = mRect.h / rangey;
          y0 = mRect.y + axAbs(miny) * scaley;
          aCanvas->drawLine(mRect.x,y0,mRect.x2(),y0);
        }

        // math func
        int   i;
        float n;
        float stepx = rangex / mRect.w;
        float cx;
        float cy;
        float x,px,py;

        //---

        x = minx;
        px = x;
        py = y0;
        aCanvas->setPenColor( aCanvas->getColor(AX_GREEN) );
        for (i=0; i<mRect.w; i++)
        {
          n = calc2(type2,x);//axSinf(x);
          cx = mRect.x+i;
          cy = y0-(n*scaley);
          //aCanvas->drawPoint( mRect.x+i, y0-(n*scaley) );
          aCanvas->drawLine( px,py,cx,cy );
          px = cx;
          py = cy;
          x += stepx;
        }

        //---

        x = minx;
        px = x;
        py = y0;
        aCanvas->setPenColor( aCanvas->getColor(AX_RED) );
        for (i=0; i<mRect.w; i++)
        {
          n = calc1(type1,x);//axSinf(x);
          cx = mRect.x+i;
          cy = y0-(n*scaley);
          aCanvas->drawLine( px,py,cx,cy );
          px = cx;
          py = cy;
          x += stepx;
        }

        aCanvas->clearClipRect();

      }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{

  private:
    axEditor*   w_Editor;
    myPainter*  w_Painter;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext)
      {
        w_Editor = NULL;
        w_Painter = NULL;
        describe("test_gain_gui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        setupEditor(640,480);
        appendParameter( new parInteger(this,"type1","", 0,   0, NUMTYPES-1, str_type1 ) );
        appendParameter( new parInteger(this,"type2","", 1,   0, NUMTYPES-1, str_type2 ) );
        appendParameter( new parFloat(  this,"min x","",-10, -10,10 ) );
        appendParameter( new parFloat(  this,"max x","", 10, -10,10 ) );
        appendParameter( new parFloat(  this,"min y","",-10, -10,10 ) );
        appendParameter( new parFloat(  this,"max y","", 10, -10,10 ) );
        appendParameter( new parFloat(  this,"n",    "", 1,  -1, 1  ) );
        //setupParameters();
        prepareParameters();
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float value = aParameter->getValue();
        if (w_Painter)
        {
          switch (index)
          {
            case 0: w_Painter->type1 = (int)value; break;
            case 1: w_Painter->type2 = (int)value; break;
            case 2: w_Painter->minx = value; break;
            case 3: w_Painter->maxx = value; break;
            case 4: w_Painter->miny = value; break;
            case 5: w_Painter->maxy = value; break;
            case 6: w_Painter->nval = value; break;
          }
          if (w_Editor) w_Editor->redrawAll();
        }
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"type1 - red"   ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"type2 - green" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"min x" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"max x" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"min y" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"max y" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"n"     ) );
        editor->connect( editor->getWidget(0), mParameters[0] );
        editor->connect( editor->getWidget(1), mParameters[1] );
        editor->connect( editor->getWidget(2), mParameters[2] );
        editor->connect( editor->getWidget(3), mParameters[3] );
        editor->connect( editor->getWidget(4), mParameters[4] );
        editor->connect( editor->getWidget(5), mParameters[5] );
        editor->connect( editor->getWidget(6), mParameters[6] );
        editor->appendWidget( w_Painter = new myPainter(editor,NULL_RECT,wa_Client) );
        editor->doRealign();

        transferParameters();

        editor->show();
        w_Editor = editor;
        return editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor->hide();
        w_Editor = NULL;
        delete editor;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
