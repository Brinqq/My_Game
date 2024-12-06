#include "glpipeline.h"
#include "gldefines.h"
#include "glattributes.h"


int GLPipeline::newVertexArrayObject(unsigned int& vao, const unsigned int vertexBuffer, const unsigned int indiceBuffer, GLAttribLayout layout){
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*3,(void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return 0;
}

void GLPipeline::draw(const unsigned int shader, const unsigned int vao, const int indices, const int drawMode){
  glUseProgram(shader);
  glBindVertexArray(vao);
  glDrawElements(drawMode, indices, GL_UNSIGNED_INT, 0);
}


void GLPipeline::clearScreen(const float R, const float G, const float B){
  glClearColor(R, G, B, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
