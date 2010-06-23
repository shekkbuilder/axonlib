#ifndef axEditorNone_included
#define axEditorNone_included
//----------------------------------------------------------------------
/*

empty/no-op editor (for AX_NOGUI)

*/

class axEditor
{
  public:
    axEditor(axInstance* aInstance) {}
    virtual ~axEditor() {}
};

// simplifies AX_MAIN/AX_ENTRYPOINT
// use NULL_EDITOR instead of axEditor to clarify that there is
// no editor

#define NULL_EDITOR axEditor

//----------------------------------------------------------------------
#endif



