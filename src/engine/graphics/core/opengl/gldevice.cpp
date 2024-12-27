#include "gldevice.h"
#include "gldefines.h"

#include "glshader.h"
#include "math/primitives.h"
#include "log.h"

int GLDevice::initialize(){
  return 0;
}


void GLDevice::shutdown(){

}


  int GLDevice::CompileShaderFromSource(unsigned int& shaderid, const char* source, int type){
  std::string shaderSource;
  GLShader::getShaderData(shaderSource, source);
  const char* ss = shaderSource.c_str();
  if(type == 0){
    shaderid = glCreateShader(GL_VERTEX_SHADER);
  }
  if(type == 1){
    shaderid = glCreateShader(GL_FRAGMENT_SHADER);
  }
  glShaderSource(shaderid, 1, &ss, NULL);
  glCompileShader(shaderid);
  if(GLShader::checkCompileStatus(shaderid)){return 1;} 
  return 0;
}



int GLDevice::newShaderProgramFromSource(unsigned int& program, const char* vertex, const char* pixel){
  std::string pixelShaderSource;
  std::string vertexShaderSource;
  GLShader::getShaderData(pixelShaderSource, pixel);
  GLShader::getShaderData(vertexShaderSource, vertex);
  const char* vss = vertexShaderSource.c_str();
  const char* pss = pixelShaderSource.c_str();
  unsigned vs = glCreateShader(GL_VERTEX_SHADER);
  unsigned ps = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vs, 1, &vss, NULL);
  glShaderSource(ps, 1, &pss, NULL);
  glCompileShader(vs);
  glCompileShader(ps);
  if(GLShader::checkCompileStatus(vs)){return 1;} 
  if(GLShader::checkCompileStatus(ps)){return 1;}
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, ps);
  glLinkProgram(program);
  glDeleteShader(vs);
  glDeleteShader(ps);
  return 0;
}

int GLDevice::newGPUBufferWithData(unsigned int& id, const void* data, const BufferDataType type, const long bytes){
  int dataType;
  int usage;
  switch(type){
  case GPU_BUFFER_VERTEX_DATA:
    dataType = GL_ARRAY_BUFFER;
    usage = GL_STATIC_DRAW;
    break; 
    case GPU_BUFFER_INDICE_DATA:
    dataType = GL_ELEMENT_ARRAY_BUFFER;
    usage = GL_STATIC_DRAW;
    break;
  }

  glGenBuffers(1, &id);
  glBindBuffer(dataType, id);
  glBufferData(dataType, bytes, data, usage);
  glBindBuffer(dataType, 0);
  return 0;
}


void GLDevice::addConstUniform4f(const unsigned int shader, const char* semantic, const void*const data){
}

void GLDevice::addConstUniform3f(const unsigned int shader, const char* semantic, Math::Vector3f data){
  glUseProgram(shader);
  int location = glGetUniformLocation(shader, semantic);
  glUniform3f(location, data.x, data.y, data.z);
}

int GLDevice::addUniform3f(const unsigned int shader, const char* semantic, Math::Vector3f data){
  glUseProgram(shader);
  int location = glGetUniformLocation(shader, semantic);
  glUniform3f(location, 0.0f, 0.0f, 1.0f);
  return location;
}

int GLDevice::addUniformMat4f(const unsigned int shader, const char* semantic, const float* const data){
  glUseProgram(shader);
  int uniformIndex = glGetUniformLocation(shader, semantic);
  glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data);
  return uniformIndex;
  glUseProgram(0);
}

  void GLDevice::updateUniformMat4(const unsigned int shader, const unsigned int uniform, const float* const data){
  glUseProgram(shader);
  glUniformMatrix4fv(uniform, 1, GL_FALSE, data);
  glUseProgram(0);
}
