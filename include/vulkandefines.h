#pragma once

#include "vulkan/vulkan.h"


struct QueueFamilyIndices{
  uint32_t graphicQueueFamilyIndex;
  uint32_t computeQueueFamilyIndex;
  uint32_t tranferQueueFamilyIndex;
};
