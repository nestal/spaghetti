# Spaghetti

Spaghetti is a tool to help understand the source code structure of a
project. It uses [libclang-c](http://clang.llvm.org/doxygen/group__CINDEX.html)
to parse the source code, and use [Qt](http://doc.qt.io/qt-5/index.html) to
render it.

![snapshot-1](./doc/snapshot1.png)
Screenshot of Spaghetti pre-v0.3 (6 Mar 2017), viewing [spaghetti-2.json](./doc/spaghetti-2.json)

## Features

* Draw UML class diagram easily by dragging classes from logical view
* Detecting class hierarchies and aggregation from source code
* Navigating to source view when double-clicking the classes in logical view
* Zooming by ctrl-mouse wheel in class diagram
* Show only used classes in logical view

## Installation and Building

See [Building Spaghetti](doc/building.md) for details.

## Version History and Download

See [Change Log](CHANGELOG.md) for a detailed list.

## License

[GNU General Public License Version 2](https://www.gnu.org/licenses/gpl-2.0.html)

See the [COPYING](./COPYING) file for details.

Any code contributed to the project must be licensed under GPLv2 to be
accepted. There is no need to assign copyright to any party. 

## Authors and Contributors

* [Nestal W.H. Wan](https://gitlab.com/nestal)
* Siu Wan (for drawing the icons)

## Help Wanted

* Algorithm for automatically arranging the items in UML class diagrams 
