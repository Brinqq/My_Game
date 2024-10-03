#include "global.h"
#include "window.h"

int main(){
  GLFWwindow* window = Window::initalizeNewWindow(1920, 1080);
  while(!Window::getWindowStatus(window)){
    Window::windowUpdate();
  }
  return 0;
}
