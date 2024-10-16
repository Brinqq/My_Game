echo "Note: Script Uses CXX enviroment variable to determine compiler"
if($null -eq $env:CXX){
  echo "No suitable compiler found, please set compiler to CXX enviroment variable"
}else{
  $Compiler = $env:CXX
  echo "Using compiler at $Compiler"
}

$Executable_Name = "game.exe"

$Bin_Path = "bin"
$Build_Path = "build"

$Fresh_Build = 0

$Linker_Flags =
$Compile_Flags = "-std=c++17"

$Source_Files = 
$Include_Directories = 

$Library_Paths =

function Create_Directories{
  New-Item -Force -Path . -Name "bin" -ItemType "directory" | Out-Null
  New-Item -Force -Path . -Name "build" -ItemType "directory" | Out-Null
  New-Item -Force -Path $Build_Path/ -Name "objects" -ItemType "directory" | Out-Null
}

function Clean{
  Remove-Item -Recurse $Bin_Path
  Remove-Item -Recurse $Build_Path
}


if(-Not(Test-Path -Path $Bin_Path)){
  echo "Required directories not found, Creating directories..."
  $Fresh_Build = 1
  Create_Directories
}

Invoke-Expression "$Compiler $compiler_Flags src\main.cpp -o $Bin_Path\$Executable_Name"
