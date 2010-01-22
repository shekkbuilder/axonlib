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

//TODO: dirty parameters, ala widgets/editor?
// så setParameter appender til dirtyList, og

/**
  \file axPlugin.h
  \brief main plugin class
*/

#ifndef axPlugin_included
#define axPlugin_included
//----------------------------------------------------------------------

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

//AX_NUMPROGS should a minimum of 1 ?,
//otherwise zero-length arrays in axPluginVst.h
#if (AX_NUMPROGS == 0)
	#undef AX_NUMPROGS
	#define AX_NUMPROGS 1
#endif

#include "axPluginVst.h"
//#include "axHostVst.h"

/**
  \class axPlugin
  \brief main plugin class

  this class in the main clas you inherit from to make a plugin...

  \sa axPluginVst
*/

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

    /// constructor
    /**
      initializes the plugin to a workable state.
      the default settings is a stereo in/out do-nothing effect
      \param audioMaster audioMaster (vst)
      \param numProgs number of programs
      \param numParams number of parameters
      \param aPlugFlags flags

    */

    axPlugin(audioMasterCallback audioMaster,long numProgs,long numParams, int aPlugFlags=0)
    : axPluginImpl(audioMaster,numProgs,numParams)
    //axPlugin(axHostVst* host,long numProgs,long numParams, int aPlugFlags=0)
    //: axPluginImpl(host,numProgs,numParams)
      {
        mPlugFlags = aPlugFlags;
        axInitialize(mPlugFlags);   // os/platform specific initialization
        canProcessReplacing();      // need this for vst sdk 2.4
        setNumInputs(2);            // defaults to 2 inputs & outputs
        setNumOutputs(2);           // aka stereo effect
        mParameters.clear();
        mName = "?";
      }

    //----------

    /// destructor
    /**
      clean up memory and things...
      parameters are automaticlly destroyed
    */
    virtual ~axPlugin()
      {
        deleteParameters();
        axCleanup(mPlugFlags);
        //TRACE("axPlugin destructor ok\n");
      }

    /// select a program
    /**
      switches the current program (set of parameters)
      copies the current parameters into the program memory,
      and updates all parameters to their correct value from the new program
      \param program  program to switch to
    */

    virtual void setProgram(VstInt32 program)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
        mCurProg = program;
        for (int i=0; i<AX_NUMPARAMS; i++) mParameters[i]->doSetValue( mPrograms[mCurProg][i] );
      }

    /// save program
    /**
      saves all parameters to program memory
    */

    virtual void saveProgram(void)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
      }

    /// duplicate program
    /**
      duplicates the parameters for the first program [0] to all other programs
      for simple initializing to a blank state
    */

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

    /// parameter change handler
    /**
      called every time a parameter has been changed, either from tweaking a widget,
      or from host automation. in your own plugin, overload this to grab the changed values.
      .
      \param aParameter the parameter that has been changed
    */

    virtual void doProcessParameter(axParameter* aParameter) {}

    //--------------------------------------------------
    // plugin handler
    //--------------------------------------------------

    /// add parameter
    /**
      append parameter to the end of the plugin parameter-list
      (and set its listener to 'this')
      \param aParameter the parameter to append
    */

    void appendParameter( axParameter* aParameter )
      {
        aParameter->mListener = this;
        //mMutex_params.lock();
        mParameters.append( aParameter );
        //mMutex_params.unlock();
      }

    //----------

    /// delete parameters
    /**
      deletes all parameters (if any)
      will automatically be called from the destructor
    */

    void deleteParameters(void)
      {
        //TODO: crash-protection :-)
        //mMutex_params.lock();
        for( int i=0; i<mParameters.size(); i++) delete mParameters[i];
        mParameters.clear();
        //mMutex_params.unlock();
      }

    //----------

    /// process parameters
    /**
      process all parameters, for initial setup
      will call doProcessParameter for all parameters
    */

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

    /// get parameter name
    /**
      get name of parameter, "lowpass", "size", ..
      \param index index of parameter
      \param text pointer to (pre-allocated) buffer to copy string to
    */

    virtual void getParameterName(VstInt32 index, char* text)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetName(text);
      }

    //----------

    /// get parameter label
    /**
      get label of parameter, "db", "ms", ..
      \param index index of parameter
      \param label pointer to (pre-allocated) buffer to copy string to
    */


    virtual void getParameterLabel(VstInt32 index, char* label)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetLabel(label);
      }

    //----------

    /// get parameter display
    /**
      get text representaion of internal value, for displauy on gui...
      \param index index of parameter
      \param text pointer to (pre-allocated) buffer to copy string to
    */

    virtual void getParameterDisplay(VstInt32 index, char* text)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doGetDisplay(text);
      }

    //----------

    /// set parameter value
    /**
      set parameter, value = 0..1
      \param index index of parameter
      \param value value (0..1) to set
    */


    virtual void setParameter(VstInt32 index, float value)
      {
        //if (index>=0 && index<mParameters.size())
          mParameters[index]->doSetValue(value);
      }

    //----------

    /// get parameter value
    /**
      get parameter value (0..1)
      \param index index of parameter
      \return parameter value (0..1)
    */


    virtual float getParameter(VstInt32 index)
      {
        //if (index>=0 && index<mParameters.size())
          return mParameters[index]->doGetValue();
        //else return 0;
      }

    //--------------------------------------------------
    // parameter listener
    //--------------------------------------------------

    /// onChange handler
    /**
      called when parameter has changed.
      default action is to just call doProcessParameter
      \param aParameter the parameter that has changed
    */


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
