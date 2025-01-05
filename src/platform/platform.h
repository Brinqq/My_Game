#pragma once
#include "global.h"
#include "defines.h"

#include "bstl/vector.h"


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

  // interface
#define PLATFORM_VULKAN_REQUIRED_INSTANCE_FLAGS VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR
void platformGetRequiredVulkanExtensions(bstl::Vector<const char*>& extensions);


void inline platformVulkanSurfaceCreate(VkInstance inst, VkSurfaceKHR* surface){
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

