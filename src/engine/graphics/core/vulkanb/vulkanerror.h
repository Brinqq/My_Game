#pragma once

#include "vulkan/vulkan.h"
#include "log.h"

enum VulkanError{
  VULKAN_ERROR_NONE = 0,
  VULKAN_ERROR_MISSING_EXTENSION,
  VULKAN_ERROR_NO_GPU_FOUND,
  VULKAN_ERROR_LENGTH
};

inline const char* vulkanErrorMessages[VULKAN_ERROR_LENGTH] = {
    "NO ERROR WHY THIS LGGING",
    "Requrired Extensions not found!",
    "Unable to find a usable device!",
};

inline VkResult result;

#define VKCALL(x) result = x; if(result != 0){vkResultWrap(result, __LINE__, __FILE__);};
#define VULKAN_ERROR_THROW(x) throwVulkanError(x, __LINE__, __FILE__)

inline void vkResultWrap(int code ,int line , const char* file){ 
  LOG_CRITICAL("VKCALL resulted in error code %i on LINE %i: in FILE %s", code, line, file);
  std::exit(2);
}

inline void throwVulkanError(const VulkanError code, int line, const char* file){
  LOG_CRITICAL("%s ERROR thrown from LINE %i: FILE %s", vulkanErrorMessages[code], line, file);
}

int vulkanErrorsInit();

void vulkanFlushInternalLogs();

VkBool32 vulkanInternalDebugMessageCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageTypes,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData);


