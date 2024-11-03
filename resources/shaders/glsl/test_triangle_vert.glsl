#version 450

layout(location = 0) vec3 inPos

void main(){
  gl_Position = vec4(positions[gl_VertexIndex] ,0.0, 1.0);
}
