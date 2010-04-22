#ifndef wdgPages_included
#define wdgPages_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgPages : public axWidget
{
  protected:
    int   mActivePage;
  public:

    wdgPages(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mActivePage = 1;
      }

    //----------

    int appendPage(axWidget* aPage)
      {
        aPage->clearFlag(wf_Active);
        aPage->clearFlag(wf_Visible);
        aPage->doSetSize(mRect.w,mRect.h);
        int index = axWidget::appendWidget(aPage);
        return index;
      }

    //----------

    void setPage(int aPage,bool redraw=false)
      {
        if (aPage>=mWidgets.size()) aPage = mWidgets.size()-1;
        //if( aPage!=mActivePage)
        //{
          for( int i=0; i<mWidgets.size(); i++ )
          {
            axWidget* wdg = mWidgets[i];
            if (i==aPage)
            {
              wdg->setFlag(wf_Active);
              wdg->setFlag(wf_Visible);
            }
            else
            {
              wdg->clearFlag(wf_Active);
              wdg->clearFlag(wf_Visible);
            }
          } //for
          //mPages->initMouseState();
          mActivePage = aPage;
          if (redraw) mListener->onRedraw(this);
        //} // !=aIndex
      }


};

//----------------------------------------------------------------------
#endif
