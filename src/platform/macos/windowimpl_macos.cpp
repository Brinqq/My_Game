#include "global.h"
#include "platform.h"
#include "windowimpl_macos.h"
#include "log.h"

#include "GLFW/glfw3.h"


#define WINDOW_DEFAULT_RES_X 1280
#define WINDOW_DEFAULT_RES_Y 720


struct WindowState;

static WindowState* pWindowState;

void windowErrorCallback(int errorName, const char* description){
  LOGGING::addLog(CRITICAL, "failed to create window");
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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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
  glfwPollEvents();
  if(glfwWindowShouldClose(state.windowHandle)){gExitFlag = true;}

}

