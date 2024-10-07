#include "Window.h"
#include "GLFW/glfw3.h"

Window::Window(){
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  windowHandle = glfwCreateWindow(windowW, windowH, "game",nullptr, nullptr);
}


Window::~Window(){
  glfwDestroyWindow(windowHandle);
}

void Window::update(){
  glfwPollEvents();
  exitFlag = glfwWindowShouldClose(windowHandle);
}
