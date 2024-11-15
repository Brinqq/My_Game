#include "global.h"
#include "platform.h"
#include "windowimpl_macos.h"
#include "defines.h"
#include "log.h"

#include "GLFW/glfw3.h"


#define WINDOW_DEFAULT_RES_X 1280
#define WINDOW_DEFAULT_RES_Y 720


struct WindowState;

static WindowState* pWindowState;

void windowErrorCallback(int errorName, const char* description){
}

void pwGetPresentationSize(int& x, int& y){
  glfwGetFramebufferSize(pWindowState->windowHandle, &x , &y);
}



int windowCreate(WindowState& state){
    const int x = WINDOW_DEFAULT_RES_X;
    const int y = WINDOW_DEFAULT_RES_Y;
    state.clientRes.x = x;
    state.clientRes.y = y;
    glfwInit();
    glfwSetErrorCallback(windowErrorCallback);
    if(_GR_BACKEND_VULKAN){
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }else{
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
      glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    }
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    state.windowHandle = glfwCreateWindow(x, y, "Game", nullptr, nullptr);
    glfwShowWindow(state.windowHandle);
    pWindowState = &state;
    return 0;
  }


void windowCleanup(WindowState& state){
  glfwDestroyWindow(state.windowHandle);
  glfwTerminate();
}

void windowUpdate(WindowState& state){
  #if _GR_BACKEND_OPENGL
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(state.windowHandle);
  #endif
  glfwPollEvents();
  if(glfwWindowShouldClose(state.windowHandle)){gExitFlag = true;}

}

