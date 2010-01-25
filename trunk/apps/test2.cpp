#define AX_APPLICATION  myApp
//#define AX_SCRIPT_STDLIB
#define AX_FLAGS 0
#define AX_DEBUG

#include "axDebug.h"
#include "axApplication.h"
#include "axScript.h"

//#include "ax.h"

//----------------------------------------------------------------------

//char* myScript = (char*)
//  ": test 10 5 + ; "      // 15
//  ": test2 3 4 6 + + ; "  // 13
//  ": main "
//  "    test test2 + . "   // 13+15 =  28
//  "    3 test * . "       // 15*3  =  45
//  "    test2 2 - . "      // 2-13  = -11
//  "    2 10 / . EXIT "    // 10/2  =  5
//  ": main2 "
//  "    16 DUP 5 DROP + . EXIT ";

//char* myScript = (char*)
//  ": SQUARE DUP * ; "
//  "$ label "
//  ""
//  ": main "
//  "  5 2 10 + + 10 > IF 3 SQUARE . ENDIF "
//  " EXIT ";

//char* myScript = (char*)
//  ": func1 10 8 > IF 1 . ENDIF ; "
//  "$ label2 5 DUP * . EXIT "
//  "$ main func1 label2 EXIT ";

char* myScript = (char*)
" : dump DUP . ; "
" $ main "
"   10 "
" $ loop1 "
"   dump "
"   1- "
"   DUP -1 > IF loop1 ELSE 666 . ENDIF "
" EXIT ";

// " : dump DUP . ; $ main 10 $ loop1 dump 1- DUP -1 > IF loop1 ELSE 666 . ENDIF EXIT ";


//----------------------------------------------------------------------

class myApp : public axApplication
{
  private:
    axScript* mScript;
    int word,label,pos;
  public:

    myApp() : axApplication() {}
    virtual ~myApp() {}

    virtual void main(void)
      {
        axInitialize(AX_FLAGS);
        mScript = new axScript();
        mScript->compile(myScript);
        label = mScript->findLabel((char*)"main");
        if (label>=0) mScript->execute( mScript->labelPos(label) );

        delete mScript;
        axCleanup(AX_FLAGS);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
