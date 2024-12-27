#version 450

layout(location = 0) out vec4 fragColor;

uniform vec3 u_color;

void main(){
  fragColor = vec4(0.0f, 0.5, 0.0f, 1.0f );
}

