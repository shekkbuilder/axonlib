#ifndef wdgKnob_included
#define wdgKnob_included
//----------------------------------------------------------------------

#include "wdg/wdgValue.h"

class wdgKnob : public wdgValue
{

  public:
    wdgKnob(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
            axString aName="", float aValue=0)
    : wdgValue(aListener,/*aId, */aRect,aAlignment,aName,aValue)
      {
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          if (mParameter)
          {
            char labelbuf[32];
            mParameter->doGetDisplay(mDisp);
            mParameter->doGetLabel(labelbuf);
            axStrcat(mDisp," ");
            axStrcat(mDisp,labelbuf);
          }
          else axFtoa(mDisp,mValue);
          mSkin->drawKnob(aCanvas,mRect,mName,mDisp,mValue/*,mSkinMode*/);
        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        //if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
        //else mListener->onCursor(cu_ArrowLeftRight);
        mListener->onCursor(cu_ArrowUpDown);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

};

//----------------------------------------------------------------------
#endif

