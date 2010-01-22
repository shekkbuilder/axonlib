/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */
 

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



