#ifndef axDemo_page_sync_included
#define axDemo_page_sync_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

//----------------------------------------------------------------------

class axDemo_page_sync : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    int   m_PlayState;
    float m_SamplePos;
    float m_SampleRate;
    float m_BeatPos;
    float m_Tempo;
    char buf[256];

  public:

    axDemo_page_sync(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
      }

    virtual ~axDemo_page_sync()
      {
      }

    void update(int aPlayState,double aSamplePos,double aSampleRate,double aBeatPos,double aTempo)
      {
        m_PlayState   = aPlayState;
        m_SamplePos   = aSamplePos;
        m_SampleRate  = aSampleRate;
        m_BeatPos     = aBeatPos;
        m_Tempo       = aTempo;
        if (isVisible())
        {
          //trace("redraw");
          //trace("mGetPlayState " << aPlayState );
          mListener->onRedraw(this);
        }
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPanel::doPaint(aCanvas,mRect);
        axColor text = aCanvas->getColor(AX_WHITE);
        int x = mRect.x;
        int y = mRect.y;
        aCanvas->setTextColor(text);
        aCanvas->drawText(x+10,y+10,"play state");  aCanvas->drawText(x+80,y+10,axItoa(buf,m_PlayState));
        aCanvas->drawText(x+10,y+25,"sample pos");  aCanvas->drawText(x+80,y+25,axFtoa(buf,m_SamplePos));
        aCanvas->drawText(x+10,y+40,"sample rate"); aCanvas->drawText(x+80,y+40,axFtoa(buf,m_SampleRate));
        aCanvas->drawText(x+10,y+55,"beat pos");    aCanvas->drawText(x+80,y+55,axFtoa(buf,m_BeatPos));
        aCanvas->drawText(x+10,y+70,"tempo");       aCanvas->drawText(x+80,y+70,axFtoa(buf,m_Tempo));
      }

};

//----------------------------------------------------------------------
#endif
