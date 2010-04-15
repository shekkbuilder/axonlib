#ifndef wdgScrollBar_included
#define wdgScrollBar_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgScrollBar : public axWidget
{
  private:
    bool is_dragging;
    float pixel_size;
    int clickx,clicky;

  protected:
    float mThumbSize;   // 0..1

  public:

    wdgScrollBar(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        is_dragging = false;
        mThumbSize = 0.5;
        //if ( aRect.w < aRect.h   ) setFlag(wf_Vertical);
        //else clearFlag(wf_Vertical);
      }

    //virtual ~wdgScrollBar()
    //  {
    //  }

    //inline void setPos(float aPos) { mPos = aPos; }

    inline void setThumbSize(float aSize)
      {
        mThumbSize = aSize;
      }

    //inline void setThumb(float aValue, float aThumb)
    //  {
    //    mValue = aValue;
    //    mThumbSize = aThumb;
    //  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        bool vert = hasFlag(wf_Vertical);
        if (mSkin) mSkin->drawScrollBar(aCanvas,mRect,mValue,vert,mThumbSize);
        //else wtrace("no skin");
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        is_dragging = true;
        int thumbsize,numpixels;
        if (hasFlag(wf_Vertical))
        {
          thumbsize = (int)((float)mRect.h*mThumbSize);
          numpixels = mRect.h - thumbsize;
        }
        else
        {
          thumbsize = (int)((float)mRect.w*mThumbSize);
          numpixels = mRect.w - thumbsize;
        }
        if (numpixels<=0) numpixels = 1;  // HACK
        pixel_size = 1.0f/(float)numpixels;
        clickx = aX;
        clicky = aY;
      }


    //----------

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        is_dragging = false;
      }

    //----------

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if (is_dragging)
        {
          int deltax = aX - clickx;
          int deltay = aY - clicky;
          if (hasFlag(wf_Vertical)) mValue += ((float)deltay * pixel_size);
          else mValue += ((float)deltax * pixel_size);
          mValue = axMax(0,axMin(mValue,1));
          mListener->onChange(this);
          clickx = aX;
          clicky = aY;
        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
        else mListener->onCursor(cu_ArrowLeftRight);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

};

//----------------------------------------------------------------------
#endif




