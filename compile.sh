#!/bin/bash

# TODO: Add reuseable object files for compilation
# TODO: Fix vulkan path env
# TODO: Fix the whole vulkan impl tbh

ProgramName=game

#TODO: Add platform detection
CXX=clang++
Platform=OSX

CompilerVersion=-std=c++17
debug="-g0 -g"
release=-g2

OutputDir=bin
BuildDir=build
ObjectOutputDir=$BuildDir/objects
LibraryFolder=lib

PlatformMacosPath=src/platform/macos/*
PlatformLinuxPath=src/platform/linux/*
PlatformWinPath=src/platform/windows/*



# SourceFiles=($(find src -name "*.cpp"))

IntermideateFiles=()
IncludeFolders=("-Iinclude/ -Isrc/platform/macos")

# glfw
GLFWPath="$LibraryFolder/glfw"
LibraryFlags="-lglfw3"
LibraryLinkerFolder="-L$GLFWPath/build/src/"
LibraryIncludes="-I$GLFWPath/include"

#vulkan
VulkanPath="$HOME/programming/vulkan/macos"
LibraryLinkerFolder+=" -L$VulkanPath/lib"
LibraryFlags+=" -lvulkan.1 -lvulkan.1.3.290"
LibraryIncludes+=" -I$VulkanPath/include"

LinkerFlags="$LibraryLinkerFolder $LibraryFlags"
CompileFlags="$CompilerVersion $debug $IncludeFolders $LibraryIncludes"

CleanBuild=0

DawrinFrameworks="-framework IOKit -framework CoreVideo -framework Cocoa -framework Metal -framework CoreFoundation -framework QuartzCore"


#TODO: implement cland compile info
ExportClangdInfo=0;


F={}

if [ "$Platform" == "OSX" ]; then
  LinkerFlags+=" $DawrinFrameworks"
  SourceFiles=($(find src -name "*.cpp" -not -path "src/platform/windows/*" -not -path "src/platform/linux/*" ))
fi




PrintInfo(){
  echo "Compiler: $CXX"
  echo Platform: $Platform
  echo "standard: $CompilerVersion"
  echo "---------------------------"
}

CreateDir(){
  mkdir $OutputDir $BuildDir $ObjectOutputDir
}

Clean(){
  rm -fr bin build
}

CompileToObject(){
  $CXX $CompileFlags -c $1 -o $ObjectOutputDir/$2

}

CreateExecutable(){
  echo "Creating Executable..."
  $CXX ${IntermideateFiles[*]} -o $OutputDir/$ProgramName $LinkerFlags
  echo "Executable Finished"
}

CompileSourceFiles(){
  for file in ${SourceFiles[*]}; do 
    name="${file##*/}"
    path="${file%/*}"
    obj="${name/%.*/.o}"
    intermideateObject=$ObjectOutputDir/${obj}
    IntermideateFiles+=($intermideateObject)
    echo "compiling $name...."
    CompileToObject $path/$name $obj
  done
  echo Compilation Finished
}

CompileLibraries(){
  cmake lib/glfw -B lib/glfw/build/
  cd lib/glfw/build && make && cd ../../../
}



if ! test -d "$OutputDir"; then
  echo "Clean Build Detected"
  echo "Creating Directorys..."
  CompileLibraries
  CreateDir
  CleanBuild=1
fi


PrintInfo
CompileSourceFiles
CreateExecutable


