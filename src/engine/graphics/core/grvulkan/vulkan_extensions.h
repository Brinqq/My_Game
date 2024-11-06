#pragma once

#include "global.h"
#include "vulkandefines.h"

struct VulkanExtensionArrays{
  std::vector<const char*> deviceExtensions;
  std::vector<const char*> instanceExtensions;
};


inline VulkanExtensionArrays gVulkanExtensions;

VKError VulkanInitDeviceAndInstanceEXT();
