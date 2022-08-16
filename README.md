# <img src="Assets/AppIcon/notebook.png" alt="notebook" width="50" height="50"></img> Note Tree
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
- At the project root, run `qmake`.
- Build the project with `make`.
- Making an Xcode project can be done with `qmake -spec macx-xcode`.

### Linux
- Download Qt5 from the package manager of the distribution.
- At the project root, run `qmake`.
- Build the project with `make`.

## Download
Compiled binaries exist for Windows, macOS, and Linux via GitHub actions. Click on the actions tab, then the most recent action to find the releases. Some oddities exist with the executables due to OS-specific bugs, GitHub actions itself, or my own lack of testing tools. These oddities are listed below grouped by platform.
### Windows
- Works flawlessly.

### macOS
- App is not code signed in any way, meaning most(?) Macs will refuse to run it, claiming it simply "cannot be opened". Signing the app will fix the problem, but the signature is tied to the machine. Have not invested enough into getting a proper code signature because of how Apple handles code signing.
  - The README file zipped with the build contains simple instructions on how to get it to run.
- Only pre-compiled x86_64 binaries exist, GitHub actions does not have the option to run on Apple Silicon and the action only gets x86 builds of Qt, not ARM. Probably possible to fix with enough tinkering, but as of now there is no native Apple Silicon build on GitHub. App works flawlessly through Rosetta 2, and building the source code on ARM produces a perfectly working version.

### Linux
- Works flawlessly (as well as AppImages work). Requires setting executable permission for the file, otherwise works completely out of the box.