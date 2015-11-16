#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: $0 [debug | release | clean]"
    exit 1
fi

set -e

# 1. Setup environment

ARPIGL_DIR=../android/arpigl
ARPIGL_ASSET_DIR=$ARPIGL_DIR/src/main/assets
ARPIGL_LIB_DIR=$ARPIGL_DIR/src/main/lib

case "$1" in
    clean) echo "Cleaning libs and assets...";
	   rm -rf *~ libs/ obj/ $ARPIGL_ASSET_DIR/arpigl-assets $ARPIGL_LIB_DIR;
	   exit 0
	   ;;
	   
    debug) NDK_DEBUG=1;
	   echo "Building libs in debug mode..."
	   ;;
    
    *) NDK_DEBUG=0;
       echo "Building libs in release mode..."
       ;;
esac

# 2. Build shared libraries

ndk-build -j16 "NDK_DEBUG=$NDK_DEBUG"
mkdir -p $ARPIGL_LIB_DIR
cp -r libs/* $ARPIGL_LIB_DIR

# 3. Build assets

mkdir -p $ARPIGL_ASSET_DIR
cd ../assets && zip -r $ARPIGL_ASSET_DIR/arpigl-assets arpigl

exit 0
