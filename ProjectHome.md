see also: [holos](http://code.google.com/p/holos/)

_**NOTE: svn is currently non-working! we're updating and changing some low-level base classes... we'll be back in a few days.. have patience**_

_...almost done..._

[discussions](http://groups.google.com/group/axonlib)

[temporary doxygen docs (updated)](http://home.broadpark.no/~toskei/axonlib/doxygen/html/)


---


list of feature for axonlib v.0.1.0

OVERVIEW:
  * c++
  * binary format abstraction for vst plugins and excutables on linux and windows
  * common look, feel, functionality among platforms
  * few external dependencies
  * compile scripts with simplified command lines for the gnu gcc compiler
  * tiny and compact binaries with no big, external libraries needed
  * cpu efficient, code prepared for compiler analysis and optimization
  * options to disable code or functionality that is not needed
  * flexible axl license, (generally gpl w/ exception for proprietary use)

CORE:
  * builtin fast memory allocator routines (and leak detection functionality)
  * builtin routines for low-level string and memory manipulation
  * heavily optimized mathematical functions and approximations
  * intuitive debugging functionality & helpers
  * static and runtime assertion

GUI:
  * hierarchial gui, flexible, skinnable, auto-layout, sizeable, moveable etc
  * resizeable window/editor (in plugin hosts as well)
  * low level gfx (gdi/xlib) canvas, surface, bitmap etc
  * partial support for antialiased, transparent lines
  * support decoding 32bit pngs from memory or from an external file
  * scalable, alpha blended bitmaps
  * mouse cursor shapes, hovering hints, mouse capture, modal widgets

DSP:
  * polyphonic voice manager and event scheduler
  * modular audio graph with connectable dsp modules
  * rbj filter bank
  * basic oversampling container
  * chamberlin state variable filter
  * rms approximation
  * envelope follower
  * basic waveform generators

PLUGINS:
  * lots of included example vst plugins
  * simplified creation and use of parameters
  * easy host tempo/sync handling for audio and midi
  * can load external files directly from plugin folder on both linux and windows

OTHER:
  * basic read/write access for external files
  * utility methods for bit manipulation and conversations
  * scripts, stack-based, 4th inspired, rudimentary compilation (bytecode)
  * builtin, random number generators
  * mersenne twister implementation, customized for small binary size impact
  * fft implementation
  * much more...

---

video capture, gui test (win7, 30.apr.2010)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=DDteKLfEPVo' target='_blank'><img src='http://img.youtube.com/vi/DDteKLfEPVo/0.jpg' width='425' height=344 /></a>

older video-capture test (13.apr.2010)

http://dl.dropbox.com/u/249632/axonlib/videos/temp1/axonlib-preview-13.apr.2010.ogv
(4.7mb)


![http://dl.dropbox.com/u/249632/axonlib/debug/test_widgets_win7%20%28copy%29.png](http://dl.dropbox.com/u/249632/axonlib/debug/test_widgets_win7%20%28copy%29.png)

---


standalone/vst-plugin library for linux/win32

![http://sites.google.com/site/ccernnaudio/_/rsrc/1263173034288/axonlib/axPreview1.png?height=480&width=451&nunsense=axPreview1.png](http://sites.google.com/site/ccernnaudio/_/rsrc/1263173034288/axonlib/axPreview1.png?height=480&width=451&nunsense=axPreview1.png)

--