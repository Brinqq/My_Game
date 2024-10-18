#pragma once
#include "global.h"



#ifdef _WIN32
#define ALLEGRO_NO_MAGIC_MAIN
  int platformStartUp();
  int initialize();
#endif

#ifdef __APPLE__ 
#include "windowimpl_macos.h"
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

  struct StateHandles{
    WindowState windowState;
  };
  
  inline StateHandles* pStateHandles;

  void windowUpdate(WindowState& state);
  int windowCreate(WindowState& state);
  void pwGetPresentationSize(int& x, int& y);

  //platform specific vulkan impl function definitions
  struct QueueFamilyIndices;

  void pvCreateSurface();
  int pvInitializeQueueFamilies(VkQueueFamilyProperties* properties, QueueFamilyIndices& indexStruct, uint32_t count);
  int pvGetRequiredInstanceExtensions(std::vector<const char*>& ext);
  int pvInitialize();

  void inline pvCreateSurface(VkInstance inst, VkSurfaceKHR* surface){
    glfwCreateWindowSurface(inst, pStateHandles->windowState.windowHandle, nullptr, surface);
  };

  //internal logic

  inline int platformInitialize(){
    pStateHandles = (StateHandles*)malloc(sizeof(StateHandles));
    windowCreate(pStateHandles->windowState);
    return 0;
  }

  inline void platformGlobalUpdate(){
    windowUpdate(pStateHandles->windowState);
  }
  
  inline void platformDeinitialize(){
    free(pStateHandles);
  }
#endif

