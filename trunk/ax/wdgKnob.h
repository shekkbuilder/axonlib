#ifndef wdgKnob_included
#define wdgKnob_included
//----------------------------------------------------------------------

//#include "axWidget.h"
#include "wdgValue.h"

class wdgKnob : public wdgValue
{
  private:
    //char buf[32];
    float val;
    char  name[32];
    char  display[32];
  public:
    int mKnobSize;

  public:

    wdgKnob(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, /*axParameter* aParameter=NULL,*/ int aSize=1)
    : wdgValue(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mKnobSize = aSize;
      }

    //--------------------------------------------------

    void drawKnob(axCanvas* aCanvas, axRect aRect, float aValue, axString aName, axString aDisplay, int aFlags)
      {
        int x = mRect.x;
        int y = mRect.y;
        // text rectangles
        aCanvas->setPenColor( AX_GREY_DARK );
        aCanvas->setBrushColor( AX_GREY_DARK );
        aCanvas->fillRect( x+16,y,   x+127,y+14 );
        aCanvas->setBrushColor( axColor(96,96,96) );
        aCanvas->fillRect( x+16,y+17,x+127,y+31 );
        aCanvas->setTextColor( AX_GREY );
        aCanvas->drawText( x+40,y,   x+127,y+14, aName, tal_Left );
        aCanvas->setTextColor( AX_GREY_LIGHT );
        aCanvas->drawText( x+40,y+17,x+127,y+31, aDisplay, tal_Left );
        // erase circle behind knob
        aCanvas->setPenColor( AX_GREY ); // get rid of win32 border
        aCanvas->setBrushColor( AX_GREY );
        aCanvas->fillCircle( x-4,y-4,x+35,y+35 );
        aCanvas->setBrushColor( axColor(112,112,112) );
        aCanvas->fillCircle( x,y,x+31,y+31 );
        // draw full outer arc
        aCanvas->setPenColor( AX_GREY_DARK );
        aCanvas->setPenWidth(4);
        aCanvas->drawArc( x+2,y+2,x+29,y+29, 0.6, 0.8 );
        //aCanvas->resetPenWidth();
        // outer arc
        aCanvas->setPenColor( AX_WHITE );
        //aCanvas->setPenWidth(4);
        aCanvas->drawArc( x+2,y+2,x+29,y+29, 0.6, aValue*0.8 );
        aCanvas->resetPenWidth();
        // draw pie (value)
        aCanvas->setPenColor( AX_GREY ); // get rid of win32 border
        aCanvas->setBrushColor( AX_GREY_LIGHT );
        aCanvas->fillArc( x+6,y+6,x+25,y+25, 0.6, aValue*0.8 );
      }

    void drawKnobSmall(axCanvas* aCanvas, axRect aRect, float aValue, axString aName, axString aDisplay, int aFlags)
      {
        int x  = mRect.x;
        int y  = mRect.y;
        //aCanvas->setBrushColor( axColor(96,96,96) );
        //aCanvas->fillRect(x+20,y,x+63,y+15);
        aCanvas->setPenColor( AX_GREY_DARK );
        aCanvas->setPenWidth( 3 );
        aCanvas->drawArc(x+1,y+1,x+15,y+15,0.6,0.8);
        aCanvas->setPenColor( AX_WHITE );
        aCanvas->drawArc(x+1,y+1,x+15,y+15,0.6,aValue*0.8);
        aCanvas->resetPenWidth();
        aCanvas->setBrushColor( axColor(192,192,192) );
        aCanvas->fillArc(x+4,y+4,x+12,y+12,0.6,aValue*0.8);
        //aCanvas->setTextColor( AX_GREY_LIGHT );
        aCanvas->setTextColor( AX_GREY_DARK );
        aCanvas->drawText(x+22,y,x+63,y+15,aDisplay,tal_Left);
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( mParameter )
        {
          name[0] = '\0';
          display[0] = '\0';
          //val = 0.8 * mParameter->mValue;
          val = mParameter->doGetValue();
          mParameter->doGetName( name );
          mParameter->doGetDisplay( display );
        }
        else
        {
          //axWidget::doPaint( aCanvas, aRect );
          val = mValue;// * 0.8;
          strcpy( name, "[name]" );
          sprintf( display, "%f", val );
        }
        switch(mKnobSize)
        {
          case 0: drawKnobSmall(aCanvas,aRect,val,name,display,0); break;
          case 1: drawKnob(aCanvas,aRect,val,name,display,0); break;
        }
      }
};

//----------------------------------------------------------------------
#endif



