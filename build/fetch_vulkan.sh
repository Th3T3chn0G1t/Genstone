#!/bin/sh

GLSLANG_REMOTE="https://github.com/KhronosGroup/glslang"
SPIRV_REFLECT_REMOTE="https://github.com/KhronosGroup/SPIRV-Reflect"
VULKAN_HEADERS_REMOTE="https://github.com/KhronosGroup/Vulkan-Headers"
VULKAN_VALIDATION_LAYERS_REMOTE="https://github.com/KhronosGroup/Vulkan-ValidationLayers"
VULKAN_LOADER_REMOTE="https://github.com/KhronosGroup/Vulkan-Loader"
MOLTENVK_REMOTE="https://github.com/KhronosGroup/MoltenVK"

VULKAN_DIR="vulkan"

GLSLANG_DIR="$VULKAN_DIR/glslang"
SPIRV_REFLECT_DIR="$VULKAN_DIR/spirv-reflect"
VULKAN_HEADERS_DIR="$VULKAN_DIR/vulkan-headers"
VULKAN_VALIDATION_LAYERS_DIR="$VULKAN_DIR/vulkan-validation-layers"
VULKAN_LOADER_DIR="$VULKAN_DIR/vulkan-loader"
MOLTENVK_DIR="$VULKAN_DIR/moltenvk"

CLONE="git clone --depth=1"

BUILD_ROOT=$(pwd)

if [ "$1" = "LNX" ] || [ "$1" = "DWN" ]
then
    PLATFORM="$1"
else
    echo "Invalid platform"
    echo "Usage: $0 [LNX|DWN] [RELEASE|DEBUG] [ENABLED|DISABLED]"
    exit 1
fi

if [ "$2" = "RELEASE" ] || [ "$2" = "DEBUG" ]
then
    BUILD_MODE="$2"
else
    echo "Invalid build mode"
    echo "Usage: $0 [LNX|DWN] [RELEASE|DEBUG] [ENABLED|DISABLED]"
    exit 1
fi

if [ "$3" = "ENABLED" ] || [ "$3" = "DISABLED" ]
then
    TOOLING="$3"
else
    echo "Invalid tooling mode"
    echo "Usage: $0 [LNX|DWN] [RELEASE|DEBUG] [ENABLED|DISABLED]"
    exit 1
fi

type cmake &> /dev/null
if [ $? -ne 0 ]
then
    echo "CMake is not installed or is not on the path"
    echo "CMake is required to build the vulkan stack"
    echo "CMake can be obtained from here: https://cmake.org/download/"
    exit 1
fi
type python3 &> /dev/null
if [ $? -ne 0 ]
then
    echo "Python3 is not installed or is not on the path"
    echo "Python3 is required to build the vulkan stack"
    echo "Python3 can be obtained from here: https://www.python.org/downloads/"
    exit 1
fi

mkdir $VULKAN_DIR || true

$CLONE $GLSLANG_REMOTE $GLSLANG_DIR
cd $GLSLANG_DIR
    mkdir External/spirv-tools || true
    mkdir build || true
    cmake -Bbuild -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" .
    make -Cbuild -j$(nproc)
cd $BUILD_ROOT

$CLONE $SPIRV_REFLECT_REMOTE $SPIRV_REFLECT_DIR
$CLONE $VULKAN_HEADERS_REMOTE $VULKAN_HEADERS_DIR
$CLONE $VULKAN_VALIDATION_LAYERS_REMOTE $VULKAN_VALIDATION_LAYERS_DIR
$CLONE $VULKAN_LOADER_REMOTE $VULKAN_LOADER_DIR

if [ "$PLATFORM" = "DWN" ]
then
    $CLONE $MOLTENVK_REMOTE $MOLTENVK_DIR
    cd $MOLTENVK_DIR
        MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS="--macos --glslang-root $GLSLANG_DIR --v-headers-root $VULKAN_HEADERS_DIR --parallel-build"
        if [ "$BUILD_MODE" = "DEBUG" ]
        then
            MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS="$MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS --debug"
        fi
        if [ "$TOOLING" = "ENABLED" ]
        then
            MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS="$MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS --asan --ubsan"        
        fi
        ./fetchDependencies $MOLTENVK_FETCH_DEPENDENCIES_ARGUMENTS
    cd $BUILD_ROOT
fi
