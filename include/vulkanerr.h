#pragma once
#include "vulkan/vulkan.h"
#include "global.h"
#include "error.h"

enum VKError{
  VULKAN_SUCCESS = 0,
  VULKAN_REQUIRED_LAYERS_NOT_FOUND = 23,
  VULKAN_NO_SUITABLE_GRAPHIC_DEVICE = 24,
  VULKAN_QUEUE_FAMILIY_INITIALIZATION_ERROR = 25,
  VULKAN_REQUIRED_DEVICE_EXTENSIONS_MISSING = 26,
  VULKAN_SWAP_CAPABILITIES_NOT_FOUND = 27,
};

#if __DEBUG
inline VkResult gVkResult;
inline VKError code;

#define VKCALL(x) gVkResult = x; if(gVkResult != VK_SUCCESS){throwVkError(gVkResult, __LINE__, __FILE__);}
#define VKCHECK(x) code = x; if( code != VULKAN_SUCCESS){logOutVKError(code);};

inline void throwVkError(VkResult code, int line, const char* file){
printf("\033[31m\033[1mVKERROR:\033[0m%i \033[1m\033[32mFILE:\033[0m%s \033[33m\033[1mLINE:\033[0m%i\n",code, file, line );
gExitFlag = true;
}
#else
#define VKCALL(x) x;
#define VKCHECK(x) x;
#endif




//TODO: Update error logging once refactor of logging class is done

struct VKMessage{
  char* ErrorType;
  char* Description;
  char* possibleSolution;
};

inline std::unordered_map<VKError, VKMessage> errorMap;

inline void logOutVKError(VKError err){
  if(err == VULKAN_REQUIRED_LAYERS_NOT_FOUND){LOG(CRITICAL, "Requested Layers not found!"); programErrorOut();}
  if(err == VULKAN_NO_SUITABLE_GRAPHIC_DEVICE){LOG(CRITICAL, "Could not find suitable GPU!"); programErrorOut();}
  if(err == VULKAN_QUEUE_FAMILIY_INITIALIZATION_ERROR){LOG(CRITICAL, "Queue family initialization failed!"); programErrorOut();}
  if(err == VULKAN_REQUIRED_DEVICE_EXTENSIONS_MISSING){LOG(CRITICAL, "Could not find required device extensions!"); programErrorOut();}
  if(err == VULKAN_SWAP_CAPABILITIES_NOT_FOUND){LOG(CRITICAL, "L!"); programErrorOut();}
}

