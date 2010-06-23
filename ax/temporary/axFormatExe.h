#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#include "axPlatform.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptor
{
  public:
    axDescriptor(axPlatform* aPlatform) { }
    virtual ~axDescriptor() { }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstance
{
  public:
    axInstance(axDescriptor* aDescriptor) { }
    virtual ~axInstance() { }
    virtual int main(int argc, char** argv) { return 0; }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

#include "axEditor.h"

//class axEditor
//{
//  public:
//    axEditor(axInstance* aInstance) { }
//    virtual ~axEditor() { }
//};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

template<class _D, class _I, class _E, class _P>
class axFormat
{
  private:
    _P* mPlatform;
    _D* mDescriptor;
    _I* mInstance;
    _E* mEditor;
  public:
    axFormat()
      {
        mPlatform   = new _P(ft_Exe);
        mDescriptor = new _D(mPlatform);
        mInstance   = new _I(mDescriptor);
        mEditor     = new _E(mInstance);
      }
    virtual ~axFormat()
      {
        delete mEditor;
        delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }
  public:
    inline axPlatform*   getPlatform(void)    { return mPlatform; }
    inline axDescriptor* getDescriptor(void)  { return mDescriptor; }
    inline axInstance*   getInstance(void)    { return mInstance; }
    inline axEditor*     getEditor(void)      { return mEditor; }
    //----------
};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

//#ifdef AX_NOGUI
//  #define AX_MAIN(_desc,_inst,_edit) AX_ENTRYPOINT(_desc,_inst,axEditor,axPlatform)
//#else
  #define AX_MAIN(_desc,_inst,_edit) AX_ENTRYPOINT(_desc,_inst,_edit,axPlatform)
//#endif

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_edit,_plat)                                        \
                                                                                      \
int main(int argc, char** argv)                                                       \
{                                                                                     \
  axFormat<_desc,_inst,_edit,_plat>* exe =  new axFormat<_desc,_inst,_edit,_plat>();  \
  _inst* instance = (_inst*)exe->getInstance();                                       \
  int result = instance->main(argc,argv);                                             \
  delete exe;                                                                         \
  return result;                                                                      \
}

//----------------------------------------------------------------------
#endif
