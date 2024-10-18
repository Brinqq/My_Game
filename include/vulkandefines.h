#pragma once

#include "global.h"

#include "vulkan/vulkan.h"


#define FORMATS_PREALLOC_ARRAY_SIZE 100
#define PRESENTMODES_PREALLOC_ARRAY_SIZE 5

struct QueueFamilyIndices{
  uint32_t graphicQueueFamilyIndex;
  uint32_t computeQueueFamilyIndex;
  uint32_t tranferQueueFamilyIndex;
  uint32_t presentQueueFamilyIndex;
};

struct SwapChainDetails{
  VkSurfaceCapabilitiesKHR capabilites;
  std::array<VkSurfaceFormatKHR,  FORMATS_PREALLOC_ARRAY_SIZE> formatArr;
  std::array<VkPresentModeKHR,PRESENTMODES_PREALLOC_ARRAY_SIZE> presentModeArr;
};
