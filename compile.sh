#!/bin/bash

ProgramName=game

#TODO: Add platform detection

CXX=clang++
Platform=OSX
BuildType=Debug
CompilerVersion=-std=c++17

OutputDir=bin
BuildDir=build
ObjectOutputDir=$BuildDir/objects
LibraryFolder=lib
PlatformMacosPath=src/platform/macos/*
PlatformLinuxPath=src/platform/linux/*
PlatformWinPath=src/platform/windows/*


if [ "$Platform" == "OSX" ]; then
  LinkerFlags=" -framework IOKit -framework CoreVideo -framework Cocoa -framework Metal -framework CoreFoundation -framework QuartzCore"
  Includes=$(find src -type d -not -path "src/platform/windows" -not -path "src/platform/linux" | sed 's/^/-I/')
  SourceFiles=($(find src -name "*.cpp" -not -path "src/platform/windows/*" -not -path "src/platform/linux/*" ))
  debug="-g0 -g -D__DUBUG"
  release=-g2
  DEFINES=""
  Includes+=" -Iinclude"
fi

# glfw
GLFWPath="$LibraryFolder/glfw"
LibraryFlags="-lglfw3"
LibraryLinkerFolder="-L$GLFWPath/build/src/"
LibraryIncludes="-I$GLFWPath/include"

# TODO: Fix vulkan path env
# TODO: Fix the whole vulkan impl tbh
#vulkan
VulkanPath="$HOME/programming/vulkan/macos"
LibraryLinkerFolder+=" -L$VulkanPath/lib"
LibraryFlags+=" -lvulkan.1 -lvulkan.1.3.290"
LibraryIncludes+=" -I$VulkanPath/include"

#TODO: Add defines add option for compiling in either debug or release


LinkerFlags+=" $LibraryLinkerFolder $LibraryFlags"
CompileFlags="$CompilerVersion $debug $Includes $LibraryIncludes $DEFINES"

CleanBuild=0

# Timer information
ScriptStartTime=$SECONDS
TotalScriptDuration=

#Log color output macors
_OUTBLACK_="\033[0;90m"
_OUTRED_="\033[0;91m"
_OUTGREEN_="\033[0;92m"
_OUTYELLOW_="\033[0;93m"
_OUTBLUE_="\033[0;94m"
_OUTPURPLE_="\033[0;95m"
_OUTCYAN_="\033[0;96m"
_OUTWHITE_="\033[0;97m"
_OUTNORM_="\033[0m"

_BOUTBLACK_="\033[1;90m"
_BOUTRED_="\033[1;91m"
_BOUTGREEN_="\033[1;92m"
_BOUTYELLOW_="\033[1;93m"
_BOUTBLUE_="\033[1;94m"
_BOUTPURPLE_="\033[1;95m"
_BOUTCYAN_="\033[1;96m"
_BOUTWHITE_="\033[1;97m"
_BOUTORANGE_="\033[1;38;5;208m"



ExportClangdInfo=1;
ClangdFile="compile_flags.txt"
rm -fr $ClangdFile
if [ $ExportClangdInfo -eq 1 ]; then
  echo -e "${_BOUTCYAN_}Exporting compile commands${_OUTNORM_}"
  echo ""

  echo $CXX >> $ClangdFile
  echo $CompilerVersion>> $ClangdFile

  for include in ${Includes[*]}; do
    echo $include >> $ClangdFile
  done

  for include in ${LibraryIncludes[*]}; do
    echo $include >> $ClangdFile

  done


  
fi


ThrowCriticalError(){
  echo -e "${_BOUTRED_}FATAL ERROR: ${_OUTNORM_}$1"
  if [ "$2" ]; then
    echo -e "${_BOUTCYAN_}NOTE: ${_OUTNORM_}$2"
  fi
  exit
}


PrintKeyValue(){
  echo -e "${_BOUTPURPLE_}[$1] ${_OUTNORM_}$2"
}


CreateDir(){
  mkdir $OutputDir $BuildDir $ObjectOutputDir $BuildDir/compile-data
}

Clean(){
  rm -fr bin build
}


CreateExecutable(){
  objects=$(find $ObjectOutputDir -name "*.o")
  $CXX $objects -o $OutputDir/$ProgramName $LinkerFlags
}


CompileFile(){
  for file in ${SourceFiles[*]}; do
    name="${file##*/}"
    path="${file%/*}"
    obj="${name/%.*/.o}"
    if [ ! -f "$ObjectOutputDir/$obj" ] || [ "$path/$name" -nt "$ObjectOutputDir/$obj" ]; then
      echo -e "${_BOUTORANGE_}[COMPILING] ${_OUTNORM_} $name"
      $CXX $CompileFlags -c $path/$name -o $ObjectOutputDir/$obj
    fi
  done
}

CompileLibraries(){
  if ! type "cmake" > /dev/null; then
    ThrowCriticalError "Cmake not found!" "Bulid script requires cmake to build extenal dependecies"
  fi
  cmake lib/glfw -B lib/glfw/build/
  cd lib/glfw/build && make && cd ../../../
}



if ! test -d "$OutputDir"; then
  echo "Creating Directorys"
  CompileLibraries
  CreateDir
  CleanBuild=1
fi


FreshBuild(){
  echo -e "${_OUTYELLOW_}Fresh build detected!${_OUTNORM_}"
  echo -e ""
}


echo -e "${_BOUTCYAN_}Configuration Info${_OUTNORM_}"
PrintKeyValue "COMPILER" $CXX
PrintKeyValue "TARGET" $Platform
PrintKeyValue "COMPILER VERSION" ${CompilerVersion:5}
PrintKeyValue "BUILD" $BuildType
PrintKeyValue "BUILD NAME" $ProgramName
echo -e ""

DynamicTimepoint=$SECONDS
echo -e "${_BOUTCYAN_}File Compilation${_OUTNORM_}"
CompileFile
if [ $? != 0 ]; then
  ThrowCriticalError "Failed to compile source files"
fi
echo -e "${_BOUTGREEN_}Compilation Finished${_OUTNORM_}"
echo -e "${_BOUTBLUE_}Compilation of files took ${_OUTNORM_}$((SECONDS-DynamicTimepoint))s"
echo -e ""


echo -e "${_BOUTCYAN_}Creating Executable${_OUTNORM_}"
CreateExecutable

if [ $? != 0 ]; then
  ThrowCriticalError "Failed during linking stage $name"
fi
echo -e "${_BOUTGREEN_}Executable created!${_OUTNORM_}"
echo -e ""



TotalScriptDuration=$((SECONDS-ScriptStartTime))s
echo -e "${_BOUTBLUE_}Script execution time took ${_OUTNORM_}$TotalScriptDuration"

echo -e ""
echo $(date +"%y%m%d %H: %M: %S") > build/compile-data/lct.txt
