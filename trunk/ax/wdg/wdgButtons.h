#ifndef wdgButtons_included
#define wdgButtons_included
//----------------------------------------------------------------------

#include "wdg/wdgButton.h"
#include "core/axArray.h"

//----------------------------------------------------------------------

// button mode
#define bm_Single   0
#define bm_Multi    1

//----------------------------------------------------------------------

typedef axArray<wdgButton*> wdgButtonList;

//----------------------------------------------------------------------

class wdgButtons : public axWidget
{
  protected:
    axWidget*     wContainer;
    int           mButtonMode;
    int           mButtonValue;
    wdgButtonList mButtons;
  public:

    wdgButtons(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener, aRect, aAlignment)
      {
        appendWidget( wContainer = new axWidget(this,NULL_RECT,wa_Client) );
        mButtonMode = bm_Single;//bm_Multi;
        mButtonValue = 0;
      }

    //--------------------------------------------------

    // accessors
    inline axWidget* getContainer(void) { return wContainer; }

    //--------------------------------------------------

    void appendButton(wdgButton* aButton)
      {
        int num = mButtons.size();
        //aButton->mId = num;
        aButton->setIndex(num);
        aButton->setListener(this);
        aButton->setMode(bm_Switch);
        mButtons.append(aButton);
        wContainer->appendWidget(aButton);
      }

    //----------

    inline void setMode(int aMode)
      {
        mButtonMode=aMode;
        if (aMode==bm_Single && mButtonValue==0) toggle(0); // hack?
      }

    //----------

    inline void setBit(int aBit) { mButtonValue|=aBit; }
    inline bool getBit(int aBit) { return (mButtonValue&aBit); }
    inline int  getVal(void)     { return mButtonValue; }

    //----------

    void toggle(int aIndex)
       {
        wdgButton* btn = mButtons[aIndex];
        bool state = btn->getState();
        if (state)
        {
          btn->setState(false);
          btn->setValue(0);
        }
        else { btn->setState(true); btn->setValue(1); }
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        //int index = aWidget->mId;
        int index = aWidget->getIndex();
        switch (mButtonMode)
        {
          case bm_Single:
            {
              //if (index!=mButtonValue)
              {
                mButtonValue = index;
                for (int i=0; i<mButtons.size(); i++)
                {
                  wdgButton* btn = mButtons[i];
                  if (btn!=aWidget) { btn->setState(false); btn->setValue(0); }
                  else { btn->setState(true); btn->setValue(1); }
                }
                mListener->onChange(this);
                //mListener->onRedraw(this);
              }
            }
            break;
          case bm_Multi:
            {
              unsigned int bit = 1 << index;
              wdgButton* btn = mButtons[index];
              if (btn->getState()) mButtonValue |= bit;
              else mButtonValue &= ~bit;
              mListener->onChange(this);
            }
            break;
        } // switch

      }

};

//----------------------------------------------------------------------
#endif
