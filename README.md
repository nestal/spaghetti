# Spaghetti

Spaghetti is a tool to help understand the source code structure of a
project. It uses [libclang-c](http://clang.llvm.org/doxygen/group__CINDEX.html)
to parse the source code, and use [Qt](http://doc.qt.io/qt-5/index.html) to
render it.

# Requirements

Spaghetti depends on the following software to build:

* A C++14 compiler (e.g. gcc)
* [libclang-c](http://clang.llvm.org/doxygen/group__CINDEX.html)
* [Qt 5.8](http://doc.qt.io/qt-5/index.html)
* [Boost 1.60](http://www.boost.org/doc/libs/1_60_0/) (older version should
work, but not tested)
* [CMake 3.6](https://cmake.org/) (any version >3 should be fine)

Spaghetti is mostly build by a Fedora 25 box, so the above versions 
are shipped with Fedora.

# License

[GNU General Public License Version 2](https://www.gnu.org/licenses/gpl-2.0.html)

See the [COPYING](./COPYING) file for details.

# Authors and Contributors

[Nestal W.H. Wan](https://gitlab.com/nestal)

# Help Wanted

* Icon/Logo
* Source code viewer with syntax highlighting
