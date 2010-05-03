#ifndef axDemo_page_bitmaps_included
#define axDemo_page_bitmaps_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

#include "voxel.h"

//----------------------------------------------------------------------

class axDemo_page_bitmaps : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    CVoxel*   mVoxel;
    axBitmap* mBitmap;
    //int x0,y0;
    float x,y;
    float a;

  public:

    axDemo_page_bitmaps(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axBitmap* aBitmap=NULL)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        mBitmap = aBitmap;
        mVoxel = new CVoxel();
        //mVoxel->makeColors();
        x = 0.5;
        y = 0.5;
        a  = 0;
      }

    virtual ~axDemo_page_bitmaps()
      {
      }

    void timer_tick(void)
      {
        if (hasFlag(wf_Visible))
        {
          x += 0.003;
          y += 0.002;
          a += 0.005;
          if (x<0)  x += 1;
          if (y<0)  y += 1;
          if (x>=1) x -= 1;
          if (y>=1) y -= 1;
          //a += 0.005;
          mListener->onRedraw(this);
        }
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPanel::doPaint(aCanvas,aRect);
        mVoxel->View(x,y,a);
        memcpy(mBitmap->getBuffer(),mVoxel->getBuffer(),320*200*4);
        aCanvas->drawBitmap(mBitmap,mRect.x,mRect.y2()-200,0,0,320,200);
      }

};

//----------------------------------------------------------------------
#endif
