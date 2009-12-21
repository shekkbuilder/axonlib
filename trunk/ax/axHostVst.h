#ifndef axHostVst_included
#define axHostVst_included
//----------------------------------------------------------------------

#include "axHost.h"

class axHostVst : public axHost
{
  protected:
    audioMasterCallback mAudioMaster;
  public:
    axHostVst() : axHost() {}
    virtual ~axHostVst() {}
};

//----------------------------------------------------------------------
#endif

