
///*
// * This file is part of Axonlib.
// *
// * Axonlib is free software: you can redistribute it and/or modify
// * it under the terms of the Axonlib License, either version 1.0
// * of the License, or (at your option) any later version.
// *
// * Axonlib is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// * See LICENSE_AX for more details.
// *
// * You should have received a copy of the Axonlib License
// * If not, see <http://axonlib.googlecode.com/>.
// */
//
//#ifndef axContext_included
//#define axContext_included
////----------------------------------------------------------------------
//
//// TODO:
//// remove this, not needed, we can use the new axFormat setup
//// instead, to get to the platform/format specific stuff that we
//// have used this for...
//
//#include "core/axDefines.h"
////#include "extern/vstsdk24/aeffect.h"
////#include "extern/vstsdk24/aeffectx.h"
//
////----------------------------------------------------------------------
//#ifdef AX_LINUX
//
//  #include <X11/Xlib.h>
//  #include <X11/Xutil.h>
//  //#include <X11/cursorfont.h>
//  //#include <pthread.h>
//
//  struct axContext
//  {
//    Display*    mDisplay;
//    Window      mWindow;//mParent;
//    AX_PTRCAST  mAudio;
//
//    axContext()
//      {
//      }
//
//    axContext(Display* aDisplay, Window aWindow, AX_PTRCAST aAudio=0)
//      {
//        mDisplay = aDisplay;
//        mWindow  = aWindow;
//        mAudio   = aAudio;
//      }
//
//  };
//
//#endif
////----------------------------------------------------------------------
//#ifdef AX_WIN32
//  #include "windows.h"
//
//  struct axContext
//  {
//    HINSTANCE   mInstance;
//    HWND        mWindow;
//    char*       mWinClassName;
//    AX_PTRCAST  mAudio;
//
//    axContext()
//      {
//      }
//
//    axContext(HINSTANCE aInstance, char* aName, AX_PTRCAST aAudio=0)
//      {
//        mInstance     = aInstance;
//        mWindow       = 0;
//        mWinClassName = aName;
//        mAudio        = aAudio;
//      }
//
//    axContext(HWND aWindow)
//      {
//        mInstance     = 0;
//        mWindow       = (HWND)aWindow;
//        mWinClassName = 0;
//        mAudio        = 0;
//        }
//
//  };
//
//#endif
////----------------------------------------------------------------------
//#endif
