#ifndef wdgLabel_included
#define wdgLabel_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgLabel : public axWidget
{
  protected:
    axString  mText;
    axColor   mTextColor;
    int       mTextAlign;
    char      txt[32];

  public:

    wdgLabel(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, /*axParameter* aParameter=NULL,*/
             axString aText="", axColor aColor=AX_GREY_DARK, int aAlign=tal_Left)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        clearFlag( wfl_Active );
        mText       = aText;
        mTextColor  = aColor;
        mTextAlign  = aAlign;//tal_Center;
        //setBackground(true,AX_GREY_DARK);
      }

    inline void setText(axString aText)
      {
        mText = aText;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint( aCanvas, aRect );
        aCanvas->setTextColor( mTextColor );
        if( mParameter )
        {
          mParameter->doGetDisplay(txt);
          aCanvas->drawText( mRect.x, mRect.y, mRect.x2(), mRect.y2(), txt, mTextAlign );
        }
        else aCanvas->drawText( mRect.x, mRect.y, mRect.x2(), mRect.y2(), mText, mTextAlign );
      }

    //virtual void doMouseDown(int aX, int aY, int aB) { wdgValue::doMouseDown(aX,aY,aB); }
    //virtual void doMouseUp(int aX, int aY, int aB) { wdgValue::doMouseUp(aX,aY,aB); }
    //virtual void doMouseMove(int aX, int aY, int aB) { wdgValue::doMouseMove(aX,aY,aB); }


};

//----------------------------------------------------------------------
#endif




