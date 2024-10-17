#include "global.h"

#include "windowimpl_macos.h"
#include "platform.h"
#include "GLFW/glfw3.h"

struct WindowState;

int windowCreate(WindowState& state){
    const int x =1920;
    const int y = 1080;
    state.clientRes.x = x;
    state.clientRes.y = y;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    state.windowHandle = glfwCreateWindow(x, y, "Game", nullptr, nullptr);
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

