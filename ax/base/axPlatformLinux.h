#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase) { /*trace("axPlatformLinux.constructor");*/ }
    virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
    virtual char* getPlatformName(void) { return (char*)"linux"; }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------
//----------------------------------------------------------------------

#include <dlfcn.h>

const char* axGetBasePath(char* path)
  {
    int len;
    char filepath[AX_MAX_PATH] = "";
    //char* path_init = path;
    #ifdef AX_FORMAT_LIB
      Dl_info dli;
      dladdr(__func__, &dli);   // dlfcn.h
      const char* slash = axStrrchr(dli.dli_fname, '/');
      if (slash)
      {
        len = (axStrrchr(dli.dli_fname, '/') + 1) - (char*)dli.dli_fname;
        axStrncpy(path, dli.dli_fname, len/*(axStrrchr(dli.dli_fname, '/') + 1) - (char*)dli.dli_fname*/);
        path[len] = 0;
      }
      else axStrcpy(path, (char*)"./");
    #endif
    #ifdef AX_FORMAT_EXE
      unsigned int rl = readlink("/proc/self/exe", filepath, sizeof(filepath));
      if (rl)
      {
        const char* slash = axStrrchr(filepath, '/');
        if (slash)
        {
          len = (axStrrchr(filepath, '/') + 1) - (char*)filepath;
          axStrncpy(path, filepath, len); // will miss the terminator 0 ?
          path[len] = 0;
        }
        else
        {
          axStrcpy(path, (char*)"./");
        }
      }
    #endif
    //axStrcat(path, '\0'); // seg.fault with this!
    //return path_init;
    return path;
  }


//----------------------------------------------------------------------
#endif
