#version 450

layout(location = 0) vec3 inPos

oid main()
  gl_Position = vec4(inPos, 1.0);
}
