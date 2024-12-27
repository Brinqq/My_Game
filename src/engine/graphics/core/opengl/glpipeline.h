#pragma once

#include "glattributes.h"

struct GLShader;

class GLPipeline{
public:
  void draw(const unsigned int shader, const unsigned int vao, const int indices, const int drawMode);
  void swapBuffer();
  int newVertexArrayObject(unsigned int& vao, const unsigned int vertexBuffer, const unsigned int indiceBuffer, GLAttribLayout layout);
  void clearScreen(const float R, const float G, const float B);
private:
};
