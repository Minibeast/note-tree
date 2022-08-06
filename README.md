# Note Tree
A simple C++ note taking application written with Qt5.

## Build
### Windows
- Install and configure Visual Studio 2019.
- Download Qt5 from [qt.io](https://qt.io/download).
- At the project root, run `qmake -tp vc`.
- Open the project.
- Running `scripts/win32-msbuild.bat` will remake the project and build in the same motion, allowing for the git version to be properly generated and displayed in the about menu correctly.

### macOS
- Install and configure Xcode.
- Download Qt5 (`brew install qt5`).
- At the project root, run `qmake -spec macx-xcode`.
- Open the project.
- Building the project with Xcode will still work, but running the `scripts/macos-xcodebuild.sh` script will add the git version in the about menu correctly.

### Linux
- Download Qt5 from the package manager of the distribution.
- At the project root, run `qmake`.
- Build the project with `make`.
