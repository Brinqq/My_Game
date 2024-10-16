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


#TODO: Fix hardcoded path
$Vulkan_SDK_Path= "S:\dev\SDK\vulkan"
$Vulkan_Include_path ="$Vulkan_SDK_Path\include\"
$Vulkan_Lib_Path=

$Includes=@("-I$Vulkan_Include_path")
$Library_Paths=@("-L$Vulkan_SDK_Path\lib\")
$Library_link=@("-lvulkan-1")

$Linker_Flags = "$Library_Paths $Library_link"
$Compile_Flags = "-std=c++17 -Iinclude\ -c $Includes"


$Source_Files = Get-ChildItem -Path src -Dir -Recurse -Exclude macos | Get-ChildItem -File -Filter *.cpp | Select -expand fullName
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

function Create_Executable{
  $Object_Files=Get-ChildItem -Path $Build_Path/objects -File -Filter *.o | Select -Expand Name
  $arr=@()
  foreach($File in $Object_Files){
    $arr += "$Build_Path/objects/"+ "$File"
  }
  Invoke-Expression "$Compiler $arr -o $Bin_Path/$Executable_Name $Linker_Flags"
  echo "Executable creation successful!"

}

function Compile_File($File){
  $substr = $File.split('\')
  $len = $substr.Length
  $Object_Name = $substr[$len-1].replace(".cpp", ".o")
  Invoke-Expression "$compiler $Compile_Flags $File -o $Build_Path/Objects/$Object_Name"
  if(-not($LastExitCode -eq 0)){
    Exit 1
  }
}

#TODO: fix main file
compile_File("src\main.cpp")
foreach($File in $Source_Files){
  Compile_File($File)
}

Create_Executable

