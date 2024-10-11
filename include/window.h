#pragma once

struct GLFWwindow;

class Window{
  private:
  public:
    GLFWwindow* windowHandle;
    int windowH = 1920;
    int windowW = 1090;
    int exitFlag = false;
    
    Window();
    ~Window();
    void update();
    int validateVulkanLayer();
};
