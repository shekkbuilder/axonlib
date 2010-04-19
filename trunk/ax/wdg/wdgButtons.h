#ifndef wdgButtons_included
#define wdgButtons_included
//----------------------------------------------------------------------

#include "wdg/wdgButton.h"
#include "core/axArray.h"

//----------------------------------------------------------------------

// bitton mode
#define bm_Single   0
#define bm_Multi    1

//----------------------------------------------------------------------

typedef axArray<wdgButton*> wdgButtonList;

class wdgButtons : public axWidget
{
  protected:
    int           mButtonMode;
    wdgButtonList mButtons;
    int           mButtonValue;
  public:

    wdgButtons(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener, aRect, aAlignment)
      {
        mButtonMode = bm_Multi;
        mButtonValue = 0;
      }

    void appendButton(wdgButton* aButton)
      {
        int num = mButtons.size();
        wtrace("num: " << num);
        aButton->mId = num;
        aButton->setMode( bm_Switch );
        mButtons.append(aButton);
        appendWidget(aButton);
      }

    inline void setMode(int aMode)
      {
        mButtonMode=aMode;
        // hack
        if (aMode==bm_Single && mButtonValue==0) toggle(0);
      }

    inline void setBit(int aBit) { mButtonValue|=aBit; }
    inline bool getBit(int aBit) { return (mButtonValue&aBit); }

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

    virtual void onChange(axWidget* aWidget)
      {
        switch (mButtonMode)
        {
          case bm_Single:
            {
              int num = aWidget->mId;
              // this fails if value = 0, and we click on first button (index=0)
              if (num!=mButtonValue)
              {
                mButtonValue = num;
                wtrace("mButtonValue: " << mButtonValue);
                for (int i=0; i<mButtons.size(); i++)
                {
                  wdgButton* btn = mButtons[i];
                  if (btn!=aWidget) { btn->setState(false); btn->setValue(0); }
                  else { btn->setState(true); btn->setValue(1); }
                }
                mListener->onChange(this);
              }
            }
            break;
          case bm_Multi:
            {
              int num = aWidget->mId;
              unsigned int bit = 1 << num;
              wdgButton* btn = mButtons[num];
              if (btn->getState()) mButtonValue |= bit;
              else mButtonValue &= ~bit;
              wtrace("mButtonValue: " << mButtonValue);
              mListener->onChange(this);
            }
            break;
        }
      }

};



//----------------------------------------------------------------------
#endif
