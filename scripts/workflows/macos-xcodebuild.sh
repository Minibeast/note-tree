#!/bin/bash
# https://stackoverflow.com/a/13555148

defines=( GIT_VERSION=$(git describe --always) )

xcodebuild -scheme "Note Tree" -configuration Release -UseModernBuildSystem=NO GCC_PREPROCESSOR_DEFINITIONS='$GCC_PREPROCESSOR_DEFINITIONS '"$(printf '%q ' "${defines[@]}")"