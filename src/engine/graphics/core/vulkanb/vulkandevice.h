#pragma once

#include "vulkandefines.h"

int vulkanCreatePhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device);
int vulkanCreateLogicalDevice(const VkPhysicalDevice& gpu);
void vulkanDeviceDestroy();
VkDevice& vulkanLogicalDeviceGet();
