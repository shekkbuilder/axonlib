//#ifndef axonlib_included
//#define axonlib_included
////----------------------------------------------------------------------
//
//#include "axArray.h"
//#include "axRect.h"
//
//class axIWindow;
//class axIContainer;
//class axIWidget;
//
//class axIEditor;
//class axIPlugin;
//class axIParameter;
//
//typedef axArray<axIWidget*> axIWidgets;
//typedef axArray<axIParameter*> axIParameters;
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//class axIParameter
//{
//  protected:
//    axIPlugin* mPlugin;
//    axIWidget* mWidget;
//};
//
////----------
//
//class axIPlugin
//{
//  protected:
//    axIEditor* mEditor;
//    axIParameters* mParameters;
//};
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//class axIWidget
//{
//  protected:
//    axIWindow*    mWindow;
//    axIContainer* mParent;
//    axIParameter* mParameter;
//  public:
//    axIWidget(axIWindow* aWindow, axIContainer* aParent)
//      {
//        mWindow = aWindow;
//        mParent = aParent;
//      }
//};
//
////----------
//
//class axIContainer : public axIWidget
//{
//  protected:
//    axIWidgets mWidgets;
//};
//
////----------
//
//class axIWindow : axIContainer
//{
//};
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//class axIEditor : public axIWindow
//{
//  protected:
//    axIPlugin* mPlugin;
//};
//
////----------------------------------------------------------------------
//#endif
