#ifdef __APPLE__
#define GLFW_INCLUDE_VULKAN
#include "global.h"
#include "window.h"
#include "GLFW/glfw3.h"
#include "log.h"

// TODO: Handle multiple possible errors when between vulkan and glfw

int Window::validateVulkanLayer(){
  if(glfwVulkanSupported() == GLFW_FALSE){
    LOG_CRITICAL("GLFW unable to obtain vulkan support");
    return 0;
  }

  return 1;
}

Window::Window(){
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  windowHandle = glfwCreateWindow(windowW, windowH, "game",nullptr, nullptr);
}


Window::~Window(){
  glfwDestroyWindow(windowHandle);
  glfwTerminate();
}

void Window::update(){
  glfwPollEvents();
  gExitFlag = glfwWindowShouldClose(windowHandle);
}

#endif
