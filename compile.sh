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

SourceFiles=($(find src/*.cpp))
IntermideateFiles=()
IncludeFolders=("include/")

LinkerFlags= 
CompileFlags="$CompilerVersion $debug -I$IncludeFolders"
CleanBuild=0

DawrinFrameworks="-framework IOKit -framework CoreVideo -framework Cocoa"


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
  mkdir $OutputDir $BuildDir $ObjectDir
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


if ! test -d "$OutputDir"; then
  echo "Clean Build Detected"
  echo "Creating Directorys..."
  CreateDir
  CleanBuild=1
fi


PrintInfo
CompileSourceFiles
CreateExecutable


