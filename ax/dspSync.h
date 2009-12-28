#ifndef dspSync_included
#define dspSync_included
//----------------------------------------------------------------------

#include "dspModule.h"

class dspSync : public dspModule
{
  public:
    void setBeats(float aBeats) {}      // setup, number of beats
    bool update(void) { return false; } // sync now?
    int  next(void) { return 0; }       // number of samples till next sync point
};

//----------------------------------------------------------------------
#endif
