//#define AX_NO_MALLOC
//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG

//----------------------------------------------------------------------
// test
//----------------------------------------------------------------------

//#include "../plugins/test/test_gain_nogui.cpp"
//#include "../plugins/test/test_gain_gui.cpp"
//#include "../plugins/test/test_gain_gui_skin.cpp"

//----------

//#include "../test/test_winsize.cpp"
//#include "../test/test_png.cpp"

//----------

// linux:
//  #define AX_XRENDER
//  build options, link with Xrender
//  -lXrender
// ----------
// win32:
#define AX_DIBSECTION
//  build options, link with libmsimg32
// ----------
#include "../test/test_xrender.cpp"


//----------------------------------------------------------------------
// demo
//----------------------------------------------------------------------

//#include "../plugins/demo/axDemo.cpp"

//----------------------------------------------------------------------
// ccernn
//----------------------------------------------------------------------

//----- no gui -----

//#include "../plugins/cernn/midi_transpose.cpp"
//#include "../plugins/ccernn/fx_distortion.cpp"
//#include "../plugins/ccernn/fx_grains.cpp"

//----- gui -----

//#include "../plugins/ccernn/fx_tempodelay.cpp"


//----------------------------------------------------------------------
// liteon
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ports
//----------------------------------------------------------------------
