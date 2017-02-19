# Building Spaghetti

# Requirements

Spaghetti depends on the following software to build:

* A C++14 compiler (e.g. [gcc](http://gcc.gnu.org))
* [libclang-c](http://clang.llvm.org/doxygen/group__CINDEX.html)
* [Qt 5.7](http://doc.qt.io/qt-5/index.html)
* [Boost 1.60](http://www.boost.org/doc/libs/1_60_0/) (older version should
work, but not tested)
* [CMake 3.6](https://cmake.org/) (any version >3 should be fine)

Spaghetti is mostly build by a Fedora 25 box, so the above versions 
are shipped with Fedora.

# Building in Linux

1. Install the prerequisites above (i.e. compiler, libclang, Qt, Boost and cmake):
`dnf install clang-devel qt5* boost-devel cmake` in Fedora Linux
1. Clone the repository: `git clone https://gitlab.com/nestal/spaghetti.git`
1. Create build directory: `mkdir /path/to/build_dir`
1. Generate makefiles: `cmake -G 'Unix Makefiles /path/to/source_dir`
1. Build: `make` (use `-j8` to speed up the build)

# Building in Win32

Building in Win32 is much more complicated than in Linux, mainly because
the required libraries are not available out-of-the-box. You need to
download them and build them. Also, if you download pre-built binaries of
the libraries, make sure to download the one that built from the same compiler
you use.

Here is the instructions for downloading/building them:

1. libclang: Pre-built binary available from [LLVM official site](http://releases.llvm.org/download.html).
Download the [Clang for Windows (64-bit)](http://releases.llvm.org/3.9.1/LLVM-3.9.1-win64.exe).
These works with MSVC 2015.
2. Qt5: Pre-built binary available from [Qt official site](https://www.qt.io/download/).
Choose the "Desktop and Mobile Application" version. The installer will let
you choose which compiler you use and download the corresponding binary.
3. Boost: it's better to build from source instead of relying on pre-built
binaries. Download the source from [Boost official site](http://www.boost.org/users/download/).
To build boost, I used the following commands:
````
booststrap
.\b2 address_model=64 install
````
After b2 finishes, the boost headers and libraries will be installed to
``c:\Boost``, unless you chose a different location.
 4. CMake: Just download from [CMake official site](https://cmake.org/download/)

## Setup CMAKE_PREFIX_PATH

You need to tell cmake where to find the libraries you just built/downloaded.
By default, cmake searches the directories specified by the variable
``CMAKE_PREFIX_PATH``. You can set use environment variables or specify
``-DCMAKE_PREFIX_PATH=path`` in the cmake command line. I prefer to use
environment variable to save some typing.

Assuming you use default locations for all the libraries above, your
 ``CMAKE_PREFIX_PATH`` should be like: ``C:\Program Files\LLVM;C:\Qt5.7\msvc2015_64;C:\Boost``
.

## Additional Note for Building with MSVC 2015 in Windows

CMake needs the -DCMAKE_GENERATOR_PLATFORM=x64 option to properly generate
project files for x64 platform. If Boost is compiled as static libraries, make
sure to pass -DBoost_USE_STATIC_LIBS=true to use them.
