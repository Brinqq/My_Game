#pragma once
#define GLFW_INCLUDE_NONE


struct GLFWwindow;

struct ClientResolution{int x = 1920; int y = 1080;};

struct WindowState{
  ClientResolution clientRes;
  GLFWwindow* windowHandle;
};
