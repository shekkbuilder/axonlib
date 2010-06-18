//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_NEW
//#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG "plugin.log"

//----------------------------------------------------------------------
// test / demo
//----------------------------------------------------------------------

//#include "../source/test/test_gain_nogui.cpp"
//#include "../source/test/test_gain_gui.cpp"
//#include "../source/test/test_gain_gui_skin.cpp"

//#include "../test/test_dmc.cpp"
//#include "../test/test_math.cpp"

//#include "../test/test_memory.cpp"
//#include "../test/test_render.cpp"
//#include "../test/test_skin.cpp"

//#include "../source/demo/axDemo.cpp"

//#define AX_SCRIPT_STDLIB
//#include "script/axScript.h"
//#include "script/axScriptStdlib.h"
//char* scr = (char*)"this is my script";
//void test(void)
//{
//  axScript* script = new axScript();
//  script->append_stdlib();
//  script->compile(scr);
//  script->execute(0);
//  delete script;
//}

//----------------------------------------------------------------------
// ccernn
//----------------------------------------------------------------------

// no gui

//#include "../source/ccernn/fx_blur.cpp"
//#include "../source/ccernn/fx_distortion.cpp"
//#include "../source/ccernn/fx_freeze.cpp"
//#include "../source/ccernn/fx_svf.cpp"
//#include "../source/ccernn/fx_wgtavg.cpp"
//#include "../source/ccernn/midi_transpose.cpp"
//#include "../source/ccernn/syn_poly.cpp"

// gui

//#include "../source/ccernn/fx_grains.cpp"
//#include "../source/ccernn/fx_tempodelay.cpp"

//----------------------------------------------------------------------
// liteon / neolit123
//----------------------------------------------------------------------

//#include "../source/liteon/liteon_biquad.cpp"
//#include "../source/liteon/liteon_tilteq.cpp"

//----------------------------------------------------------------------
// ports
//----------------------------------------------------------------------

//#include "../source/ports/sstillwell_eventhorizon.cpp"
#include "../source/ports/mverb.cpp"
