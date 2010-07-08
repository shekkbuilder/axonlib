#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase) { /*trace("axPlatformLinux.constructor");*/ }
    virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------
//----------------------------------------------------------------------


//BUG: missing terminating 0
const char* axGetBasePath(char* path)
  {
    char* path_init = path;
    #ifdef AX_FORMAT_LIB
      Dl_info dli;
      dladdr(__func__, &dli);   // dlfcn.h
      const char* slash = axStrrchr(dli.dli_fname, '/');
      if (slash) axStrncpy(path, dli.dli_fname, (axStrrchr(dli.dli_fname, '/') + 1) - (char*)dli.dli_fname);
      else axStrcpy(path, (char*)"./");
    #endif
    #ifdef AX_FORMAT_EXE
      char filepath[AX_MAX_PATH] = "";
      unsigned int rl = readlink("/proc/self/exe", filepath, sizeof(filepath));
      if (rl)
      {
        const char* slash = axStrrchr(filepath, '/');
        if (slash) axStrncpy(path, filepath, (axStrrchr(filepath, '/') + 1) - (char*)filepath);
        else axStrcpy(path, (char*)"./");
      }
    #endif
    axStrcat(path, '\0');
    return path_init;
  }


//----------------------------------------------------------------------
#endif
