#include "glshader.h"
#include "gldefines.h"

#include "log.h"
#include "fstream"

int GLShader::getShaderData(std::string& data, const char* filePath){
  std::string line;
  std::ifstream stream(filePath);
  if(!stream.is_open()){
    LOG_ERROR("could not open file at path: %s", filePath);
    return 1;
  }
  while(getline(stream, line)){
    data.append(line+"\n");
  }
  return 0;
}


int GLShader::checkCompileStatus(unsigned int shader){
  int result;
  glGetShaderiv(shader,GL_COMPILE_STATUS, &result);
  if(!result){
    int len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    char* msg = (char*)alloca(len* sizeof(char));
    glGetShaderInfoLog(shader, len, &len,msg);
    LOG_ERROR("ShaderError - %s", msg);
    printf("here");
    return 1;
  }
  return 0;
}
