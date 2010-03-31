#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

//class axSkin
//{
//  protected:
//    axPens    mPens;
//    axBrushes mBrushes;
//    axFonts   mFonts;
//  public:
//    axSkin(axCanvas* aCanvas) {}
//    virtual ~axSkin() {}
//    inline axPen*   getPen(int aIndex)    { return mPens[aIndex]; }
//    inline axBrush* getBrush(int aIndex)  { return mBrushes[aIndex]; }
//    inline axFont*  getFont(int aIndex)   { return mFonts[aIndex]; }
//    inline void appendPen(axPen* aPen)        { mPens.append(aPen); }
//    inline void appendBrush(axBrush* aBrush)  { mBrushes.append(aBrush); }
//    inline void appendFont(axFont* aFont)     { mFonts.append(aFont); }
//};
//
////----------------------------------------------------------------------
//
//class axDefSkin : public axSkin
//{
//  public:
//    axDefSkin(axCanvas* aCanvas)
//    : axSkin(aCanvas)
//      {
//        mPens.append( aCanvas->createPen(AX_GREY_DARK) );
//        mPens.append( aCanvas->createPen(AX_GREY) );
//        mPens.append( aCanvas->createPen(AX_GREY_LIGHT) );
//        mBrushes.append( aCanvas->createBrush(AX_GREY_DARK) );
//        mBrushes.append( aCanvas->createBrush(AX_GREY) );
//        mBrushes.append( aCanvas->createBrush(AX_GREY_LIGHT) );
//        mFonts.append( aCanvas->createFont("default",AX_GREY_LIGHT) );
//      }
//    virtual ~axDefSkin()
//      {
//        //for (int i=0; i<mBrushes.size(); i++) { delete mBrushes[i]; }
//        //for (int i=0; i<mPens.size(); i++) { delete mPens[i]; }
//      }
//};

//----------------------------------------------------------------------
#endif
