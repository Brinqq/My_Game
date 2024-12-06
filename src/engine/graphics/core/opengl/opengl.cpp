#include "opengl.h"
#include "gldefines.h"
#include "log.h"

#include "platform.h"
#include <fstream>
#include <alloca.h>
#include "gldevice.h"

static unsigned int buffer;
static unsigned int vao;
static unsigned int attrib;
static unsigned int vertShader;
static unsigned int fragShader;
static unsigned int shaderProgram;
static unsigned int vsi;
static unsigned int fsi;


static const GLfloat triArr[9]{
  -0.5, -0.5, 0.0f,
  0.5, -0.5, 0.0f,
  0.0, 0.5, 0.0f,
};

static void logOutOpenGLInfo(){
  LOG_INFO("OpenGL version %s",glGetString(GL_VERSION));
  LOG_INFO("GLSL version %s",glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG_INFO("OpenGL Vendor %s",glGetString(GL_VENDOR));
}

int readInShader(std::string& shader, const char* filePath){
  LOG_INFO("Reading shader at %s", filePath);
  std::string line;
  std::ifstream stream(filePath);
  if(!stream.is_open()){
    LOG_ERROR("could not open file at path: %s", filePath);
    return 1;
  }
  while(getline(stream, line)){
    shader.append(line+"\n");
  }
  return 0;
};

void openglTest(){
  logOutOpenGLInfo();
  GLDevice device;
  std::string fragmentShader;
  std::string vertexShader;
  readInShader(fragmentShader, "resources/shaders/gl/testshaderfrag.glsl");
  readInShader(vertexShader,   "resources/shaders/gl/testshadervert.glsl");
  const char* vs = vertexShader.c_str();
  const char* ps = fragmentShader.c_str();
  vsi = glCreateShader(GL_VERTEX_SHADER);
  fsi = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fsi, 1, &ps, NULL);
  glShaderSource(vsi, 1, &vs, NULL);
  glCompileShader(vsi);
  glCompileShader(fsi);
  int result;
  glGetShaderiv(vsi,GL_COMPILE_STATUS, &result);
  if(!result){
    int len;
    glGetShaderiv(vsi, GL_INFO_LOG_LENGTH, &len);
    char* msg = (char*)alloca(len* sizeof(char));
    glGetShaderInfoLog(vsi, len, &len,msg);
    printf("VSShaderError - %s", msg);
  }
  glGetShaderiv(fsi,GL_COMPILE_STATUS, &result);
  if(!result){
    int len;
    glGetShaderiv(fsi, GL_INFO_LOG_LENGTH, &len);
    char* msg = (char*)alloca(len*sizeof(char));
    glGetShaderInfoLog(fsi, len, &len,msg);
    printf("PSShaderError - %s", msg);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, fsi);
  glAttachShader(shaderProgram, vsi);
  glLinkProgram(shaderProgram);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &buffer);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(triArr), triArr, GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*3,(void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
}

void openglUpdate(){
  glClearColor(0.1f, 0.1f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shaderProgram);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glfwSwapBuffers(pStateHandles->windowState.windowHandle);
}

void openglDeinitialize(){
  glDeleteBuffers(1, &buffer);
  glDeleteVertexArrays(1, &vao);
}
