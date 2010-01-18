#define AX_APPLICATION  myApp
#define AX_SCRIPT_STDLIB
#define AX_FLAGS 0
#define AX_DEBUG

#include "axDebug.h"
#include "axApplication.h"
#include "axScript.h"

//----------------------------------------------------------------------

char* myScript = (char*)
  ": test 10 5 + ; "      // 15
  ": test2 3 4 6 + + ; "  // 13
  ": main "
  "    test test2 + . "   // 13+15 =  28
  "    3 test * . "       // 15*3  =  45
  "    test2 2 - . "      // 2-13  = -11
  "    2 10 / . EXIT "    // 10/2  =  5
  ": main2 "
  "    0 . EXIT ";

//----------------------------------------------------------------------

class myApp : public axApplication
{
  private:
    axScript* mScript;
    int word,pos;
  public:

    myApp() : axApplication() {}
    virtual ~myApp() {}

    virtual void main(void)
      {
        axInitialize(AX_FLAGS);
        mScript = new axScript();
        /*int codesize = */mScript->compile(myScript);
        mScript->dumpCode();

        printf("----------\n");
        word = mScript->findWord((char*)"main");
        if (word>=0) mScript->execute( mScript->wordPos(word) );

        printf("----------\n");
        word = mScript->findWord((char*)"main2");
        if (word>=0) mScript->execute( mScript->wordPos(word) );

        delete mScript;
        axCleanup(AX_FLAGS);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
