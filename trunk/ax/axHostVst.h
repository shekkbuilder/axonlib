#ifndef axHostVst_included
#define axHostVst_included
//----------------------------------------------------------------------

//#include "axHost.h"

class axHostVst// : public axHostBase
{
  protected:
    audioMasterCallback mAudioMaster;

  public:

    axHostVst(audioMasterCallback aAudioMaster)
    //: axHostBase()
      {
        mAudioMaster = aAudioMaster;
      }

    virtual ~axHostVst() {}

    inline audioMasterCallback getAudioMaster(void) { return mAudioMaster; }
};

//----------------------------------------------------------------------
#endif



