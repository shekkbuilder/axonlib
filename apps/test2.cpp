#define AX_APPLICATION  myApp
#define AX_SCRIPT_STDLIB
#define AX_FLAGS 0
#define AX_DEBUG

#include "axDebug.h"
#include "axApplication.h"
#include "axScript.h"

//----------------------------------------------------------------------

char* myScript = (char*)
  ": test 10 5 + ; "
  ": test2 3 4 6 + + ; "
  ": main"
  "    test test2 + . "
  "    1 test + . "
  "    test2 . ";

//----------------------------------------------------------------------

class myApp : public axApplication
{
  private:
    axScript* mScript;
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
        int entry = mScript->findWord((char*)"main");
        int pos = 0;
        if (entry>=0) pos = mScript->wordPos(entry);
        mScript->execute(pos);
        delete mScript;
        axCleanup(AX_FLAGS);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
