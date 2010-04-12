#ifndef wdgScrollBox_included
#define wdgScrollBox_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"
#include "wdg/wdgScrollBar.h"

class wdgScrollBox : public axContainer
{
  protected:
    axContainer*  wContainer;
    wdgScrollBar* wScrollBar;
    int           mScrolledX; // how much we've already scrolled
    int           mScrolledY; // -_"--
    float         mVisible;   // how much of the content is visible (calculated in doRealign)

  public:

    wdgScrollBox(axWidgetListener* aListener, /*int aID, */axRect aRect, int aAlignment=wa_None)
    : axContainer(aListener, /*aID, */aRect, aAlignment)
      {
        axContainer::appendWidget( wScrollBar  = new wdgScrollBar(this,axRect(0,0,15,0),wa_Right ) );
        axContainer::appendWidget( wContainer  = new axContainer( this,NULL_RECT,       wa_Client ) );
        wScrollBar->setFlag(wf_Vertical);
        wScrollBar->setThumbSize(0.5);
        mScrolledX = mScrolledY = 0;
        mVisible = 1;
        //doRealign();
      }

    //virtual ~wdgScrollBox()
    //  {
    //  }

    //--------------------------------------------------

    // accessors

    inline axContainer*   getContainer(void) { return wContainer; }
    inline wdgScrollBar*  getScrollBar(void) { return wScrollBar; }

    //virtual int        getNumWidgets(void)   { return wContainer->getNumWidgets(); }
    //virtual axWidget*  getWidget(int aIndex) { return wContainer->getWidget(aIndex); }

    inline void setContainer(axContainer* c)
    {
      delete wContainer;
      wContainer = c;
      mWidgets[1] = c;
      doRealign();
    }

    //--------------------------------------------------

    void unscroll(void)
      {
        doScroll(-mScrolledX,-mScrolledY);
        //wScrollBar->doSetValue(0);
        wScrollBar->setValue(0);
      }

    //----------

    void calc_thumbsize(void)
      {
        float height  = (float)wContainer->getRect().h;       // size of container
        float content = (float)wContainer->getContent().h;    // size of content
        if (content>height)
        {
          mVisible = height / content;
          wScrollBar->setThumbSize(mVisible);
        }
        else
        {
          mVisible = 1;
          wScrollBar->setThumbSize(1);
        }
      }

    //----------

    void set_scrollpos(float x, float y)
      {
        float scrollable = (float)wContainer->getContent().h * (1-mVisible);
        int yscroll = (int)(scrollable*y);
        int dy = yscroll - mScrolledY;
        doScroll(0,dy);
      }

    //--------------------------------------------------

//    virtual int appendWidget(axWidget* aWidget)
//      {
//        int index = mWidgets.size();
//        aWidget->doSetSkin(mSkin,false); // inherit skin
//        aWidget->doSetPos( mRect.x + aWidget->getRect().x, mRect.y + aWidget->getRect().y );
//        mWidgets.append(aWidget);
//        return index;
//      }



    virtual int appendWidget(axWidget* aWidget)
      {
        //wtrace("wdgScrollBox.appendWidget");
        aWidget->doSetSkin(mSkin,false); // inherit skin
        int num = wContainer->appendWidget(aWidget);
        //wtrace("  " << aWidget->getRect().x << "," << aWidget->getRect().y );
        return num;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void doScroll(int dX, int dY)
      {
        for( int i=0; i<wContainer->getNumWidgets(); i++ )
        {
          axWidget* wdg = wContainer->getWidget(i);
          wdg->doSetPos( wdg->getRect().x - dX, wdg->getRect().y - dY );
        }
        mScrolledX += dX;
        mScrolledY += dY;
      }

    //----------

    virtual void doResize(int aW, int aH)
      {
        axContainer::doResize(aW,aH);
        calc_thumbsize();
      }

    //----------

    virtual void doRealign(void)
      {
        //float y = wScrollBar->doGetValue();
        float y = wScrollBar->getValue();
        unscroll();
        axContainer::doRealign();
        calc_thumbsize();
        set_scrollpos(0,y);
        //wScrollBar->doSetValue(y);
        wScrollBar->setValue(y);
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawPanel(aCanvas,mRect);
        axContainer::doPaint(aCanvas,aRect);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==wScrollBar)
        {
          //float val = aWidget->doGetValue();
          float val = aWidget->getValue();
          float scrollable = (float)wContainer->getContent().h * (1-mVisible);
          int i = (int)(scrollable*val);
          int dy = i - mScrolledY;
          doScroll(0,dy);
          mListener->onChange(this);
        }
        else
          mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif

