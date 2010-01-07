#ifndef axPlugin_included
#define axPlugin_included
//----------------------------------------------------------------------
//TODO: dirty parameters, ala widgets/editor?
// så setParameter appender til dirtyList, og

#include "axDefines.h"
#include "axString.h"
#include "axParameter.h"
//#include "axWidget.h"

//#include "axMutex.h"

//----------------------------------------------------------------------

//class axPluginBase
//{
//};

//----------------------------------------------------------------------

#include "axPluginVst.h"
//#include "axHostVst.h"

class axPlugin  : public axPluginImpl,
                  public axParameterListener//,
                  //public axWidgetListener
{
  public:
    //axMutex       mMutex_params;
    int           mPlugFlags;
    axParameters  mParameters;
    axString      mName;

  public:

    axPlugin(audioMasterCallback audioMaster,long numProgs,long numParams, int aPlugFlags=0)
    : axPluginImpl(audioMaster,numProgs,numParams)
    //axPlugin(axHostVst* host,long numProgs,long numParams, int aPlugFlags=0)
    //: axPluginImpl(host,numProgs,numParams)
      {
        mPlugFlags = aPlugFlags;
        axInitialize(mPlugFlags);
        canProcessReplacing();
        setNumInputs(2);
        setNumOutputs(2);
        mParameters.clear();
        mName = "?";
      }

    //----------

    virtual ~axPlugin()
      {
        deleteParameters();
        axCleanup(mPlugFlags);
        //TRACE("axPlugin destructor ok\n");
      }

    virtual void setProgram(VstInt32 program)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
        mCurProg = program;
        for (int i=0; i<AX_NUMPARAMS; i++) mParameters[i]->doSetValue( mPrograms[mCurProg][i] );
      }

    virtual void saveProgram(void)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
      }

    virtual void duplicatePrograms(void)
      {
        for (int i=1; i<AX_NUMPROGS; i++)
        {
          strcpy( mProgramNames[i],mProgramNames[0]);
          for (int j=0; j<AX_NUMPARAMS; j++) mPrograms[i][j] = mPrograms[0][j];
        }

      }


    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter) {}

    //--------------------------------------------------
    // plugin handler
    //--------------------------------------------------

    void appendParameter( axParameter* aParameter )
      {
        aParameter->mListener = this;
        //mMutex_params.lock();
        mParameters.append( aParameter );
        //mMutex_params.unlock();
      }

    //----------

    void deleteParameters(void)
      {
        //TODO: crash-protection :-)
        //mMutex_params.lock();
        for( int i=0; i<mParameters.size(); i++) delete mParameters[i];
        mParameters.clear();
        //mMutex_params.unlock();
      }

    //----------

    void processParameters(void)
      {
        for( int i=0; i<mParameters.size(); i++ ) doProcessParameter( mParameters[i] );
      }

    //----------------------------------------
    // dirty
    //----------------------------------------
    //
    //void clearDirty(void)
    //  {
    //    mDirtyList.clear(false);
    //  }
    //
    //----------
    //
    //void appendDirty(axWidget* aWidget)
    //  {
    //    for( int i=0; i<mDirtyList.size(); i++ ) if( mDirtyList[i]==aWidget ) return;
    //    mDirtyList.append(aWidget);
    //  }
    //
    //----------
    //
    //void redrawDirty(void)
    //  {
    //    TRACE("redrawDirty\n");
    //    int num = mDirtyList.size();
    //    for( int i=0; i<num; i++ )
    //    {
    //      axWidget* wdg = mDirtyList[i];
    //      redrawWidget(wdg);
    //    }
    //    clearDirty();
    //    TRACE("...redrawDirty ok\n");
    //  }


    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void getParameterName(VstInt32 index, char* text)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetName(text);
      }

    //----------

    virtual void getParameterLabel(VstInt32 index, char* label)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetLabel(label);
      }

    //----------

    virtual void getParameterDisplay(VstInt32 index, char* text)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetDisplay(text);
      }

    //----------

    virtual void setParameter(VstInt32 index, float value)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doSetValue(value);
      }

    //----------

    virtual float getParameter(VstInt32 index)
      {
        //if (index>=0 && index<mParameters.size())
          return mParameters[index]->doGetValue();
        //else return 0;
      }

    //--------------------------------------------------
    // parameter listener
    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        //TRACE("axPlugin.onChange (par)\n");
        //if( mEditor ) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------------------------------------
    // widget listener
    //----------------------------------------

};

//----------------------------------------------------------------------
#endif
