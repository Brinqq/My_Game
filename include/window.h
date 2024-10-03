
class GLFWwindow;

namespace Window{
  GLFWwindow* initalizeNewWindow(int windowH, int windowW);
  void windowUpdate();
  int getWindowStatus(GLFWwindow* windowHandle);
}
