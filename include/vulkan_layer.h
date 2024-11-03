#pragma once

#include "defines.h"
#include "vulkandefines.h"


struct VulkanInstanceLayers{
  const int layerCount;
  const char* layers[];
};


//TODO: Make layers Configurable at runtime?
#if __DEBUG
inline VulkanInstanceLayers gRequiredInstanceLayers{1, {"VK_LAYER_KHRONOS_validation"}};
#else
inline VulkanInstanceLayers gRequiredIntanceLayers{0, {}};
#endif

VKError vulkanValidateLayers();

