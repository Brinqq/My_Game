#!/bin/bash

Compiler=glslc

Root_Folder=$(pwd)
Shader_Folder=$Root_Folder/resources/shaders/glsl

Vertex_Shader_Flags="-fshader-stage=vertex"
Fragment_Shader_Flags="-fshader-stage=fragment"

Compile_VS_Shader(){
  $Compiler $Vertex_Shader_Flags $1 -o $2
}

Compile_PS_Shader(){
  $Compiler $Fragment_Shader_Flags $1 -o $2

}

Compile_VS_Shader "$Root_Folder/resources/shaders/glsl/test_triangle_vert.glsl" "bin/shaders/spv/tt_vert.spv"
Compile_PS_Shader "$Root_Folder/resources/shaders/glsl/test_triangle_frag.glsl" "bin/shaders/spv/tt_frag.spv"

