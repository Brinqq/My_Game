#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main(){
  gl_Position = u_proj * u_view * u_model * vec4(pos.x, pos.y, pos.z, 1.0f);
}
