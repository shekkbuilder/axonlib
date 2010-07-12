/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axCpu_included
#define axCpu_included

#include "axDefines.h"
#include "axStdlib.h"

#ifndef axGetBit
  #define axGetBit(x, bit) ( 1 & ((x) >> (bit)) )
#endif

// fPIC compatible
#define _AXCPU_EBX_REG "r"
#define _AXCPU_EBX_STORE  \
  "pushl %%ebx;"          \
  "cpuid;"                \
  "movl %%ebx, %1;"       \
  "popl %%ebx;"

class axCpu
{
  private:
    unsigned char isCalled;
    char          cpustringbuf[256];

  public:
    unsigned int  caps;
    unsigned char _SSE3,
                  _SSSE3,
                  _FPU,
                  _CMOV,
                  _SSE,
                  _SSE2,
                  _SSE4A,
                  _SSE5,
                  _MMX,
                  _MMXEXT,
                  _3DNOW,
                  _3DNOWEXT;

    axCpu(void)   { isCalled = 0; caps = 0; }
    ~axCpu(void)  {}

    // get cpu specifiction
    void
    axCpuId(const int fcall=33139, int* eax=NULL, int* ebx=NULL,
    int* ecx=NULL, int* edx=NULL)
    {
      if (fcall == 33139)  // 33139 = some default number
      {
        isCalled = 1;
        int a, b, c, d;
        // -----------------
        // 0x00000001
        __asmv
        (
          _AXCPU_EBX_STORE
          : "=a" (a), "="_AXCPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x00000001) : "cc"
        );
        _SSE3   = axGetBit(c, 0);
        _SSSE3  = axGetBit(c, 9);
        _FPU    = axGetBit(d, 0);
        _CMOV   = axGetBit(d, 15);
        _SSE    = axGetBit(d, 25);
        _SSE2   = axGetBit(d, 26);
        // -----------------
        // 0x80000001
        __asmv
        (
          _AXCPU_EBX_STORE
          : "=a" (a), "="_AXCPU_EBX_REG"" (b),
            "=c" (c), "=d" (d) : "a" (0x80000001) : "cc"
        );
        _SSE4A    = axGetBit(c, 4);
        _SSE5     = axGetBit(c, 11);
        _MMX      = axGetBit(d, 23);
        _MMXEXT   = axGetBit(d, 22);
        _3DNOW    = axGetBit(d, 31);
        _3DNOWEXT = axGetBit(d, 30);
      }
      // user defined call
      // -----------------
      else
        __asmv
        (
          _AXCPU_EBX_STORE
          : "=a" (*eax), "="_AXCPU_EBX_REG"" (*ebx),
            "=c" (*ecx), "=d" (*edx) : "a" (fcall) : "cc"
        );
    }

    // get cpu caps

    unsigned int
    axCpuCaps(void)
    {
      if (!isCalled)
        axCpuId();
      if (_SSE3)      caps |= 0x0001;
      if (_SSSE3)     caps |= 0x0002;
      if (_FPU)       caps |= 0x0004;
      if (_CMOV)      caps |= 0x0008;
      if (_SSE)       caps |= 0x0010;
      if (_SSE2)      caps |= 0x0020;
      if (_SSE4A)     caps |= 0x0040;
      if (_SSE5)      caps |= 0x0080;
      if (_MMX)       caps |= 0x0100;
      if (_MMXEXT)    caps |= 0x0200;
      if (_3DNOW)     caps |= 0x0400;
      if (_3DNOWEXT)  caps |= 0x0800;
      return caps;
    }

    // axCpuCapsString
    char*
    axCpuCapsString(void)
    {
      if (!isCalled)
        axCpuId();
      cpustringbuf[0] = 0;
      if (_SSE3)     axStrcat(cpustringbuf,(char*)"sse3 ");
      if (_SSSE3)    axStrcat(cpustringbuf,(char*)"ssse3 ");
      if (_FPU)      axStrcat(cpustringbuf,(char*)"fpu ");
      if (_CMOV)     axStrcat(cpustringbuf,(char*)"cmov ");
      if (_SSE)      axStrcat(cpustringbuf,(char*)"sse ");
      if (_SSE2)     axStrcat(cpustringbuf,(char*)"sse2 ");
      if (_SSE4A)    axStrcat(cpustringbuf,(char*)"sse4a ");
      if (_SSE5)     axStrcat(cpustringbuf,(char*)"sse5 ");
      if (_MMX)      axStrcat(cpustringbuf,(char*)"mmx ");
      if (_MMXEXT)   axStrcat(cpustringbuf,(char*)"mmxext ");
      if (_3DNOW)    axStrcat(cpustringbuf,(char*)"3dnow ");
      if (_3DNOWEXT) axStrcat(cpustringbuf,(char*)"3dnowext ");
      return cpustringbuf;
    }

    // call rdtsc()
    #ifdef __AX64__
      inline unsigned long long rdtsc(void)
      {
        unsigned low, high;
        __asmv ( "cpuid;" "rdtsc;" : "=a" (low), "=d" (high) );
        return ( (low) | ( (unsigned long)(high) << 32 ) );
      }
    #endif
    #ifdef __AX32__
      inline unsigned long long rdtsc(void)
      {
        unsigned long long val;
        __asmv ( "cpuid;" "rdtsc;" : "=A" (val) );
        return val;
      }
    #endif

};

#endif // axCpu_included

/**
 * @file axCpu.h
 * \brief axCpu class
 *
 * usage:
 * \code
 * axCpu mycpu;
 * mycpu.axCpuId();
 * if (mycpu._FPU) { printf("processor has FPU\n"); }
 *
 * int eax, ebx, ecx, edx;
 * mycpu.axCpuId(0x00000001, &eax, &ebx, &ecx, &edx);
 * // do something with variable 'eax'
 *
 * unsigned long long start_time, end_time, diff;
 *
 * start_time = mycpu.rdtsc();
 * // do something
 * end_time = mycpu.rdtsc();
 * diff = end_time - start_time;
 * // note will not work correctly for multi-core
 *
 * \endcode
 * for more information see: <br>
 * http://en.wikipedia.org/wiki/CPUID <br>
 * http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/25481.pdf <br>
 * http://www.intel.com/Assets/PDF/appnote/241618.pdf
 */