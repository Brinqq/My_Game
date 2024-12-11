#pragma once
#include "defines.h"

#if _WINDOW_API_GLFW
class GLFWwindow;
int inputSystemInitialize();
void inputSystemShutDown();
void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void inputSystemUpdate();
#endif
