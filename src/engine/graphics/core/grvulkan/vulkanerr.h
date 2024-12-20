#pragma once
#include "vulkan/vulkan.h"
#include "global.h"
#include "error.h"



enum VKError{
  VULKAN_SUCCESS = 0,
  VULKAN_REQUIRED_LAYERS_NOT_FOUND = 23,
  VULKAN_NO_SUITABLE_GRAPHIC_DEVICE = 24,
  VULKAN_QUEUE_FAMILY_INITIALIZATION_ERROR = 25,
  VULKAN_REQUIRED_DEVICE_EXTENSIONS_MISSING = 26,
  VULKAN_SWAP_CAPABILITIES_NOT_FOUND = 27,
  VULKAN_SWAP_CHAIN_VALIDATION_ERROR = 28,
  VULKAN_PRESENTATION_NOT_SUPPORTED_ERROR = 29,
  VULKAN_SWAPCHAIN_INITIALIZATION_ERROR = 30,
  VULKAN_IMAGEVIEW_INITIALIZATION_ERROR = 31
};

#if __DEBUG

#define VULKAN_VALIDATION_OUPUT_FILE "debug/VULKAN_VALIDATION_OUPUT_FILE.txt"

inline VkResult gVkResult;
inline VKError code;

#define VKCALL(x) gVkResult = x; if(gVkResult != VK_SUCCESS){throwVkError(gVkResult, __LINE__, __FILE__);}
#define VKCHECK(x) code = x; if( code != VULKAN_SUCCESS){logOutVKError(code);};
#define VKCH(x) code = x; if( code != VULKAN_SUCCESS){logOutVKError(code); return 1;};

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
  if(err == VULKAN_REQUIRED_LAYERS_NOT_FOUND){LOG_CRITICAL("Requested Layers not found!");}
  if(err == VULKAN_NO_SUITABLE_GRAPHIC_DEVICE){LOG_CRITICAL("Could not find suitable GPU!");}
  if(err == VULKAN_QUEUE_FAMILY_INITIALIZATION_ERROR){LOG_CRITICAL("Queue family initialization failed!");}
  if(err == VULKAN_REQUIRED_DEVICE_EXTENSIONS_MISSING){LOG_CRITICAL("Could not find required device extensions!");}
  if(err == VULKAN_SWAP_CAPABILITIES_NOT_FOUND){LOG_CRITICAL( "L!"); programErrorOut();}
  if(err == VULKAN_SWAP_CHAIN_VALIDATION_ERROR){LOG_CRITICAL( "Failure while validating swap chain support");}
  if(err == VULKAN_PRESENTATION_NOT_SUPPORTED_ERROR){LOG_CRITICAL( "GPU does not support presentation requirments");}
  if(err == VULKAN_SWAPCHAIN_INITIALIZATION_ERROR){LOG_CRITICAL( "Failure to initialize swap chain during vulkan view system initialization");}
  if(err == VULKAN_IMAGEVIEW_INITIALIZATION_ERROR){LOG_CRITICAL( "Failure to initialize view images during vulkan view system initialization");}
  LOG_CRITICAL("Unkown error");
  programErrorOut();
}

