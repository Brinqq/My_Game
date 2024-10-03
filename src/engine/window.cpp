#include "defines.h"
#include "GLFW/glfw3.h"

namespace Window{

GLFWwindow* initalizeNewWindow(int windowH, int windowW){
  glfwInit();
  return glfwCreateWindow(windowW, windowH, GAME_NAME, nullptr, nullptr)        ;
}

void windowUpdate(){
  glfwPollEvents();
}

int getWindowStatus(GLFWwindow* windowHandle){
  return glfwWindowShouldClose(windowHandle);
}

}

