#!/bin/bash

# TODO: Add reuseable object files for compilation

ProgramName=game

CXX=clang++
Platform=OSX

CompilerVersion=-std=c++17
debug=-g0
release=-g2

OutputDir=bin
BuildDir=build
ObjectOutputDir=$BuildDir/objects
LibraryFolder=lib

SourceFiles=($(find src -name "*.cpp"))
IntermideateFiles=()
IncludeFolders=("include/")

GLFWPath="$LibraryFolder/glfw"
LibraryFlags="-lglfw3"
LibraryIncludes="-I$GLFWPath/include"
LibraryLinkerFolder="-L$GLFWPath/build/src/"

LinkerFlags="$LibraryLinkerFolder $LibraryFlags"
CompileFlags="$CompilerVersion $debug -I$IncludeFolders $LibraryIncludes"

CleanBuild=0

DawrinFrameworks="-framework IOKit -framework CoreVideo -framework Cocoa -framework Metal -framework CoreFoundation -framework QuartzCore"


F={}

if [ "$Platform" == "OSX" ]; then
  LinkerFlags+=" $DawrinFrameworks"
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


